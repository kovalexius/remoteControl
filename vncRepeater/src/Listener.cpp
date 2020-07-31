#include "Listener.h"

#include <iostream>

#include <poll.h>
#include <string.h>

#include "common.h"

Listener::Listener(const int _port1, 
					const int _port2) : 
							m_left(_port1), 
							m_right(_port2)
{
}

void Listener::doWork()
{
	Socket& socketLeft = m_left.Listen(&m_right);
	Socket& socketRight = m_right.Listen(&m_left);

	struct pollfd fds[2];
	
	fds[0].fd = socketLeft.Get();
	fds[0].events = POLLIN;
	fds[1].fd = socketRight.Get();
	fds[1].events = POLLIN;

	while(true)
	{
		auto ret = poll(fds, 2, TIMEOUT);
		if(ret == -1)
		{
			std::cout << "poll failed: \'" << strerror(errno) << "\'" << std::endl;
		}
		else if(ret == 0)
		{}
		else
		{
			if(fds[0].revents & POLLIN)
			{
				fds[0].revents = 0;
				m_left.Accept();
			}
			if(fds[1].revents & POLLIN)
			{
				fds[1].revents = 0;
				m_right.Accept();
			}
		}
	}
}