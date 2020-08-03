#include "multiplexer.h"

#include <exception>
#include <iostream>

#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "ip_tools.h"


constexpr int MAXEVENTS = 65536;

CMultiplexer::CMultiplexer()
{
	m_efd = epoll_create(1);
    if (m_efd <0)
        std::cout << "epoll_create() failed" << std::endl; 
}

CMultiplexer::~CMultiplexer()
{
	shutdown(m_efd, SHUT_RDWR);
	close(m_efd);
}

bool CMultiplexer::AddSocket(Socket& _socket)
{
    struct epoll_event event;
    event.data.fd = _socket.Get();
    event.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;

    if(epoll_ctl(m_efd, EPOLL_CTL_ADD, _socket.Get(), &event) == -1)
    {
        std::cout << "epoll_ctl failed: " << strerror(errno) << std::endl;
        throw std::runtime_error(strerror(errno));
    }

    std::lock_guard<std::recursive_mutex> lk(m_mutex);
    m_socks.insert({_socket.Get(), _socket});
    std::cout << __FUNCTION__ << " _socket: " << _socket.Get() << std::endl;
}

bool CMultiplexer::RemoveSocket(const Socket& _socket)
{
    std::cout << __FUNCTION__ << " _socket: " << _socket.Get() << std::endl;

    if(epoll_ctl(m_efd, EPOLL_CTL_DEL, _socket.Get(), NULL) == -1)
    {
        std::cout << "epoll_ctl failed: " << strerror(errno) << std::endl;
        return false;
        throw std::runtime_error(strerror(errno));
    }

    std::lock_guard<std::recursive_mutex> lk(m_mutex);
    m_socks.erase(_socket.Get());
    return true;
}

void CMultiplexer::SubscribeOnReceive(OnReceive _onReceive)
{
    m_receive_handle = _onReceive;
}

void CMultiplexer::SubscribeOnHup(OnHup _onHup)
{
    m_onHup_handle = _onHup;
}

void CMultiplexer::Worker()
{
    std::cout << __FUNCTION__ << " threadid: " << std::this_thread::get_id() << std::endl;

	struct epoll_event events[MAXEVENTS];
    while(m_isLaunched)
    {   
        //std::cout << "epoll_wait checking" << std::endl;
        int n = epoll_wait(m_efd, events, MAXEVENTS, TIMEOUT);
        //int n = epoll_wait(m_efd, events, MAXEVENTS, -1);
        for(int i = 0; i < n; i++)
        {
            std::cout << "epoll signaled" << std::endl;
            
            if(events[i].events & EPOLLHUP || events[i].events & EPOLLRDHUP)        // somebody was hanged up, aka closed peer
            {
                process_hup_events(events[i]);
            }

            if(events[i].events & EPOLLIN)         // ready to read or accept connections
            {
                int sock = events[i].data.fd;

                std::lock_guard<std::recursive_mutex> lk(m_mutex);
	            auto it = m_socks.find(sock);
                if(it != m_socks.end())
                if(m_receive_handle)
                {
                    m_receive_handle(it->second);
                }
            }
        }
    }
}

void CMultiplexer::process_hup_events(const struct epoll_event &_evnt)
{
    int cur_sock = _evnt.data.fd;

	std::lock_guard<std::recursive_mutex> lk(m_mutex);
    auto it = m_socks.find(cur_sock);
    if(it != m_socks.end())
    {
		if(::epoll_ctl(m_efd, EPOLL_CTL_DEL, cur_sock, NULL) == -1)
    	{
			std::cout << "epoll_ctl failed: " << strerror(errno) << std::endl;
        	throw std::runtime_error(strerror(errno));
    	}
        if(m_onHup_handle)
            m_onHup_handle(it->second);
        m_socks.erase(it);
        return;
    }
}