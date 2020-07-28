#include "common.h"
#include "Socket.h"

#include <stdexcept>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>


constexpr int LISTEN_BACKLOG = 1024;


bool bind_socket(const Socket& _socket,
					const std::string& _iface_addr,
                    const int _iface_port)
{
    sockaddr_in bind_addr;
    memset(&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;

	if(_iface_addr.empty())
	{
		std::cout << "INADR_ANY" << std::endl;
		bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		std::cout << "iface_addr: \'" << _iface_addr << "\'" << std::endl;
		bind_addr.sin_addr.s_addr = inet_addr(_iface_addr.c_str());
	}
    
	bind_addr.sin_port = htons(_iface_port);
    
	if(bind(_socket.Get(), (struct sockaddr*)&bind_addr, sizeof(bind_addr)) < 0)
	{
        throw std::runtime_error(strerror(errno));
		return false;
	}

	return true;
}

bool listen_socket(const Socket& _socket)
{
	if(listen(_socket.Get(), LISTEN_BACKLOG) < 0)
	{
		throw std::runtime_error(strerror(errno));
		return false;
	}
	return true;
}

bool accept_socket(const Socket& _listenSocket, Socket& _dataSocket, std::string& _peerAddr)
{
	struct sockaddr_in local;
    socklen_t addrlen = sizeof(local);
    int data_sock = accept(_listenSocket.Get(), (struct sockaddr *)&local, &addrlen);
	if(data_sock > 0)
		_dataSocket.Assign(data_sock);
	else
	{
		throw std::runtime_error(strerror(errno));
		return false;
	}

	char* strAddr = inet_ntoa(local.sin_addr);
	if(strAddr)
		_peerAddr.assign(strAddr);

	return true;
}