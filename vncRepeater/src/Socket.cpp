#include "Socket.h"

#include <iostream>

#include <sys/types.h>
#ifdef WIN32
#include "winsock2.h"
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

#include "socket_impl.h"

#ifdef WIN32
#define CloseSocket closesocket
#else
#define CloseSocket close
#endif

Socket::Socket() : m_socket(std::make_shared<SocketImpl>())
{}

//*
Socket::Socket(const int _socket) : m_socket(std::make_shared<SocketImpl>(_socket))
{
}
/**/

Socket::~Socket()
{}

// Копирующая семантика
Socket::Socket(const Socket& _other) : m_socket(_other.m_socket)
{
    std::cout << __FUNCTION__ << " copy constructor sock: " << _other.m_socket->Get() << std::endl;
}

Socket& Socket::operator=(const Socket& _other)
{
    std::cout << __FUNCTION__ << " copy operator sock: " << _other.m_socket->Get() << std::endl;
    m_socket = _other.m_socket;
    return *this;
}

// Семантика перемещения
//*
Socket::Socket(Socket&& _other)
{
    std::cout << __FUNCTION__ << " move constructor sock: " << _other.m_socket->Get() << std::endl;
	m_socket = _other.m_socket;
	_other.m_socket.reset();
}
Socket& Socket::operator=(Socket&& _other)
{
    std::cout << __FUNCTION__ << " move operator sock: " << _other.m_socket->Get() << std::endl;
	if(m_socket == _other.m_socket)
		return *this;

	m_socket = _other.m_socket;
	_other.m_socket.reset();
	return *this;
}
/**/

void Socket::Assign(const int _socket)
{
    if(m_socket)
	    m_socket->Assign(_socket);
}

bool Socket::operator<(const Socket& _other) const
{
	return m_socket < _other.m_socket;
}

int Socket::Get() const
{
	return m_socket->Get();
}