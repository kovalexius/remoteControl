#include "socket_impl.h"

#include <iostream>

#include <sys/types.h>
#ifdef WIN32
#include "winsock2.h"
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

#ifdef WIN32
#define CloseSocket closesocket
#else
#define CloseSocket close
#endif


SocketImpl::SocketImpl() : m_socket(-1)
{
	if((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
        std::cout << "Couldn't create connection: new socket doesn't created" << std::endl;
	}
	std::cout << __FUNCTION__ << " m_socket: " << m_socket << std::endl;
}

SocketImpl::SocketImpl(const int _socket) : m_socket(_socket)
{
}

SocketImpl::~SocketImpl()
{
	std::cout << __FUNCTION__ << " m_socket: " << m_socket << std::endl;
	closeSocket();
}


void SocketImpl::closeSocket()
{
	if(m_socket != -1)
	{
		std::cout << "Close socket: " << m_socket << std::endl;
		CloseSocket(m_socket);
		m_socket = -1;
	}
}

void SocketImpl::Assign(const int _socket)
{
	closeSocket();
	m_socket = _socket;
}

int SocketImpl::Get() const
{
	return m_socket;
}