#include "interface.h"

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

#include "common.h"

// https://habr.com/ru/company/infopulse/blog/415259/

constexpr int MESSAGE_MAX_LEN = 100;
const uint32_t MAXEVENTS = 65536;

Iface::Iface(const int _port) : m_port(_port)
{
}

Socket& Iface::Listen()
{
	bind_socket(m_listenSocket, std::string(), m_port);
	listen_socket(m_listenSocket);

	return m_listenSocket;
}

bool Iface::Accept()
{
	Socket dataSocket;
	std::string peerAddr;
	if(accept_socket(m_listenSocket, dataSocket, peerAddr))
		std::cout << "peerAddr: " << peerAddr << std::endl;
	else
	{
		std::cout << "failed errno: " << strerror(errno) << std::endl;
		return false;
	}
	


	
}

/*
void data_thread()
{
    struct epoll_event events[MAXEVENTS];
    while(m_data_thread_isrun)
    {   
        int n = epoll_wait(m_efd, events, MAXEVENTS, -1);
        for(int i = 0; i < n; i++)
        {
            std::string str_event;
            ip_tools::epoll_events_to_str(events[i].events, str_event);   //print
            //std::cout << "events: " << str_event << std::endl;
            
            if(events[i].data.fd == m_spfd)        // self-pipe event
                break;
            
            if(events[i].events & EPOLLHUP || events[i].events & EPOLLRDHUP )        // somebody was hanged up, aka closed peer
            {
                _process_hup_events(events[i]);
                continue;
            }
            if(events[i].events & EPOLLIN)         // ready to read or accept connections
                _process_input_events(events[i]);
            if(events[i].events & EPOLLOUT )        // ready to write or connect was established
                _process_output_events(events[i]);
        }
        sleep(1);
        std::cout << "thread is run: " << m_data_thread_isrun << std::endl;
    }
}
*/