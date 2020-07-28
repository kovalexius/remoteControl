#include "id_receiver.h"

#include <exception>

#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

#include "ip_tools.h"

constexpr int MESSAGE_MAX_LEN = 100;
constexpr int MAXEVENTS = 65536;

CIdReceiver::CIdReceiver()
{
	m_efd = epoll_create(1);
}

CIdReceiver::~CIdReceiver()
{
	shutdown(m_efd, SHUT_RDWR);
	close(m_efd);
}

bool CIdReceiver::AddSocket(Socket& _socket)
{
	std::lock_guard<std::mutex> lk(m_mutex);

    struct epoll_event event;
    event.data.fd = _socket.Get();
    event.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;

    if(epoll_ctl(m_efd, EPOLL_CTL_ADD, _socket.Get(), &event) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
	//m_socks.emplace(_socket.Get(), std::move(_socket));
    m_socks.insert({_socket.Get(), std::move(_socket)});
}

void CIdReceiver::Worker()
{
	struct epoll_event events[MAXEVENTS];
    while(m_isLaunched)
    {   
        int n = epoll_wait(m_efd, events, MAXEVENTS, -1);
        for(int i = 0; i < n; i++)
        {
            std::string str_event;
            ip_tools::epoll_events_to_str(events[i].events, str_event);   //print
            
            if(events[i].events & EPOLLHUP || events[i].events & EPOLLRDHUP)        // somebody was hanged up, aka closed peer
            {
                process_hup_events(events[i]);
                continue;
            }
			/*
            if(events[i].events & EPOLLIN)         // ready to read or accept connections
                _process_input_events(events[i]);
            if(events[i].events & EPOLLOUT )        // ready to write or connect was established
                _process_output_events(events[i]);
				*/
        }
        sleep(1);
    }
}

void CIdReceiver::process_hup_events(const struct epoll_event &evnt)
{
    int cur_sock = evnt.data.fd;
    auto it = m_socks.find(cur_sock);
    if(it != m_socks.end())
    {
		if(::epoll_ctl(m_efd, EPOLL_CTL_DEL, cur_sock, NULL) == -1)
    	{
        	throw std::string(strerror(errno));
    	}

        //::close(cur_sock);
        //m_error_handle(CONNECT_ERROR::DISCONNECTED, cur_sock);
        std::lock_guard<std::mutex> lk(m_mutex);
        m_socks.erase(it);
        return;
    }
}

/*
void read()
{
	char buf[MESSAGE_MAX_LEN];
	std::string msg;
    auto recv_size = read(dataSocket.Get(), buf, MESSAGE_MAX_LEN);
    if(recv_size > 0)
    msg.insert(0, buf, recv_size);
}
*/