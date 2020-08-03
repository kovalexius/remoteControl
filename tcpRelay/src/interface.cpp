#include "interface.h"

#include <iostream>

#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>

#include "common.h"

#include "ip_tools.h"


const std::string IDNAME("ID:");

Iface::Iface(const int _port) : m_port(_port), 
                                m_neighbor(nullptr)
{
}

Socket& Iface::Listen(Iface* _neighbor)
{
    m_neighbor = _neighbor;
	bind_socket(m_listenSocket, std::string(), m_port);
	listen_socket(m_listenSocket);
    CMultiplexer::OnReceive onReceive = std::bind(&Iface::onRead, this, std::placeholders::_1);
    //CMultiplexer::OnReceive onReceive = std::bind(&Iface::onRead, this);
    m_multiplexer.SubscribeOnReceive(onReceive);
    m_multiplexer.SubscribeOnHup(std::bind(&Iface::onHup, this, std::placeholders::_1));
    m_multiplexer.Run();

	return m_listenSocket;
}

bool Iface::Accept()
{
	Socket dataSocket(-1);
	std::string peerAddr;
	if(accept_socket(m_listenSocket, dataSocket, peerAddr))
    {
		//std::cout << "peerAddr: " << peerAddr << std::endl;
    }
	else
	{
		std::cout << "failed errno: " << strerror(errno) << std::endl;
		return false;
	}

	m_multiplexer.AddSocket(dataSocket);
}

bool Iface::CheckIdRemove(const std::string& _idVal, Prebuffer& _prebufer)
{
    std::cout << __FUNCTION__ << std::endl;
    if(m_listId.CheckIdAndRemove(_idVal, _prebufer.m_socket))
    {
        m_multiplexer.RemoveSocket(_prebufer.m_socket);
        m_listMsg.PopMessage(_prebufer.m_socket, _prebufer.m_buffer);
        return true;
    }
    return false;
}

void Iface::onHup(const Socket& _socket)
{
    std::cout << __FUNCTION__ << " _socket: " << _socket.Get() << std::endl;
    m_listId.RemoveSocket(_socket);
}

void Iface::onRead(const Socket& _socket)
{
    std::string msg;
	read_socket(_socket, msg);
    std::cout << __FUNCTION__ << " threadid: " << std::this_thread::get_id() << " msg: " << msg << std::endl;
    // НУЖНО!!! Накопить буфер с сообщениями!
    // Как только появится ID:<id> удалить эту строку из буфера с сообщениями, связать пиры и отправить им накопленное.

    // Парсинг ID
    auto varStr = msg.substr(0, IDNAME.length());
    if(varStr == IDNAME)
    {
        auto idStr = msg.substr(IDNAME.length());
        processId(idStr, _socket);
    }
    else
        m_listMsg.PushMessage(_socket, msg);
}

void Iface::processId(const std::string& _id, const Socket& _socket)
{
    // Запомнить id шник и сокет.
    // Если такой id есть, то запустить оба сокета в poll в отдельном потоке
    if(m_neighbor)
    {
        Prebuffer neighborsock;
        if(m_neighbor->CheckIdRemove(_id, neighborsock))
        {
            std::cout << "connecting peers" << std::endl;
            Prebuffer currentPrebuffer;
            currentPrebuffer.m_socket = _socket;
            m_listMsg.PopMessage(_socket, currentPrebuffer.m_buffer);
            m_multiplexer.RemoveSocket(_socket);
            std::thread exchangeThr(passThroughThread, currentPrebuffer, neighborsock);
            exchangeThr.detach();
        }
        else
        {
            std::cout << "id: " << _id << " received" << std::endl;
            m_listId.AddSocket(_id, _socket);
        }
    }
}

void passThroughThread(const Prebuffer _prebuffer1, const Prebuffer _prebuffer2)
{
    // https://it.wikireading.ru/7073
    // Разрыв соединения возвращает POLLIN

    if(!_prebuffer1.m_buffer.empty())
        write(_prebuffer2.m_socket.Get(), _prebuffer1.m_buffer.data(), _prebuffer1.m_buffer.size());

    if(!_prebuffer2.m_buffer.empty())
        write(_prebuffer1.m_socket.Get(), _prebuffer2.m_buffer.data(), _prebuffer2.m_buffer.size());

    struct pollfd fds[2];
	
	fds[0].fd = _prebuffer1.m_socket.Get();
	fds[0].events = POLLIN||POLLOUT|POLLHUP;
	fds[1].fd = _prebuffer2.m_socket.Get();
	fds[1].events = POLLIN||POLLOUT|POLLHUP;

	while(g_isLoop)
	{
		auto ret = poll(fds, 2, TIMEOUT);
		if(ret == -1)
		{
			std::cout << "poll failed: \'" << strerror(errno) << "\'" << std::endl;
            return;
		}
		else if(ret == 0)
		{}
		else
		{
            /*
            std::string str_event;
            ip_tools::poll_revents_to_str(fds[0].revents, str_event);   //to string
            std::cout << __FUNCTION__ << " fd0: " << str_event << std::endl;
            ip_tools::poll_revents_to_str(fds[1].revents, str_event);   //to string
            std::cout << __FUNCTION__ << " fd1: " << str_event << std::endl;
            */

			if(fds[0].revents & POLLIN)
			{
				fds[0].revents = 0;
				std::string msg;
                if(read_socket(_prebuffer1.m_socket, msg))
                {
                    // Когда считывающая половина соединения TCP закрывается (например, если получен сегмент FIN), 
                    // это также считается равнозначным обычным данным, и последующая операция чтения возвратит нуль.
                    if(msg.size() == 0)
                        goto end;
                    write(_prebuffer2.m_socket.Get(), msg.data(), msg.size());
                }
                else
                    goto end;
			}
			if(fds[1].revents & POLLIN)
			{
				fds[1].revents = 0;
				std::string msg;
                if(read_socket(_prebuffer2.m_socket, msg))
                {
                    if(msg.size() == 0)
                        goto end;
                    write(_prebuffer1.m_socket.Get(), msg.data(), msg.size());
                }
                else
                    goto end;
			}
            if(fds[0].revents & POLLHUP || fds[0].revents & POLLRDHUP || fds[0].revents & POLLERR ||
                fds[1].revents & POLLHUP || fds[1].revents & POLLRDHUP || fds[1].revents & POLLERR)
            {
                std::cout << "poll hup: " << strerror(errno) << std::endl;
                //shutdown(_prebuffer1.m_socket.Get(), SHUT_RDWR);
                //shutdown(_prebuffer2.m_socket.Get(), SHUT_RDWR);
                goto end;
            }
            sleep(1);
		}
	}

end:
    shutdown(_prebuffer1.m_socket.Get(), SHUT_RDWR);
    shutdown(_prebuffer2.m_socket.Get(), SHUT_RDWR);
}