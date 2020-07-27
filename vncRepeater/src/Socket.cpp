#include "Socket.h"

#include <stdexcept>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#ifdef WIN32
#define CloseSocket closesocket
#else
#define CloseSocket close
#endif

Socket::Socket()
{
	if((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        std::cout << "Couldn't create connection: new socket doesn't created" << std::endl;
}

Socket::Socket(const int _socket) : m_socket(_socket)
{}

Socket::~Socket()
{
	std::cout << "Destructor" << std::endl;
	closeSocket();
}

void Socket::closeSocket()
{
	if(m_socket != -1)
	{
		std::cout << "Close socket" << std::endl;
		CloseSocket(m_socket);
		m_socket = -1;
	}
}

Socket& Socket::operator=(const int _socket)
{
	closeSocket();
	m_socket = _socket;
	return *this;
}

bool Socket::operator<(const Socket& _other)
{
	return m_socket < _other.m_socket;
}

int Socket::Get() const
{
	return m_socket;
}