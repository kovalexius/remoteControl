#include "Socket.h"

#include <stdexcept>
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

Socket::Socket() : m_socket(-1)
{
	if((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
        std::cout << "Couldn't create connection: new socket doesn't created" << std::endl;
	}
	std::cout << __FUNCTION__ << " m_socket: " << m_socket << std::endl;
}

Socket::Socket(const int _socket) : m_socket(_socket)
{}

Socket::~Socket()
{
	std::cout << "Destructor" << std::endl;
	closeSocket();
}

// Семантика перемещения
Socket::Socket(Socket&& _other)
{
	m_socket = _other.m_socket;
	_other.m_socket = -1;
}
Socket& Socket::operator=(Socket&& _other)
{
	if(this == &_other)
		return *this;

	m_socket = _other.m_socket;
	_other.m_socket = -1;
	return *this;
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

void Socket::Assign(const int _socket)
{
	closeSocket();
	m_socket = _socket;
}

bool Socket::operator<(const Socket& _other)
{
	return m_socket < _other.m_socket;
}

int Socket::Get() const
{
	return m_socket;
}