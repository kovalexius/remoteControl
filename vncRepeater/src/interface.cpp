#include "interface.h"

#include <iostream>

#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>

#include "common.h"


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

bool Iface::CheckId(const std::string& _idVal, Socket& _outSock)
{
    std::cout << __FUNCTION__ << std::endl;
    return m_listId.CheckIdAndRemove(_idVal, _outSock);
}

void Iface::onHup(const Socket& _socket)
{
    std::cout << __FUNCTION__ << " _socket: " << _socket.Get() << std::endl;
    m_listId.RemoveSocket(_socket);
}

void Iface::onRead(Socket _socket)
{
    std::cout << __FUNCTION__ << " threadid: " << std::this_thread::get_id() << std::endl;

    std::string msg;
	read_socket(_socket, msg);
    std::cout << "msg: " << msg << std::endl;

    // Парсинг ID
    auto varStr = msg.substr(0, IDNAME.length());
    if(varStr == IDNAME)
    {
        auto idStr = msg.substr(IDNAME.length());
        Socket newSock(_socket);
        // Запомнить id шник и сокет.
        // Если такой id есть, то запустить оба сокета в poll в отдельном потоке
        if(m_neighbor)
        {
            Socket neighborsock(-1);
            if(m_neighbor->CheckId(idStr, neighborsock))
            {
                // _socket и neighborsock спарить в poll и в отдельном потоке пускай прокидывают друг другу.
                std::cout << "connecting peers" << std::endl;
                sleep(1);
                m_multiplexer.RemoveSocket(_socket);
                std::thread exchangeThr(passThroughThread, newSock, neighborsock);
                exchangeThr.detach();
                m_listId.RemoveSocket(idStr);
            }
            else
            {
                std::cout << "id: " << idStr << " received" << std::endl;
                sleep(1);
                m_listId.AddSocket(idStr, newSock);
            }
        }
    }
}


void passThroughThread(const Socket _sock1, const Socket _sock2)
{
    struct pollfd fds[2];
	
	fds[0].fd = _sock1.Get();
	fds[0].events = POLLIN|POLLHUP|POLLERR;
	fds[1].fd = _sock2.Get();
	fds[1].events = POLLIN;

	while(true)
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
			if(fds[0].revents & POLLIN)
			{
				fds[0].revents = 0;
				std::string msg;
                if(read_socket(_sock1, msg))
                    write(_sock2.Get(), msg.data(), msg.length());
			}
			if(fds[1].revents & POLLIN)
			{
				fds[1].revents = 0;
				std::string msg;
                if(read_socket(_sock2, msg))
                    write(_sock1.Get(), msg.data(), msg.length());
			}
            if(fds[0].revents & POLLHUP || fds[0].revents & POLLRDHUP || fds[0].revents & POLLERR ||
                fds[1].revents & POLLHUP || fds[1].revents & POLLRDHUP || fds[1].revents & POLLERR)
            {
                std::cout << "poll hup: " << strerror(errno) << std::endl;
                shutdown(_sock1.Get(), SHUT_RDWR);
                shutdown(_sock2.Get(), SHUT_RDWR);
                return;
            }
		}
	}
}