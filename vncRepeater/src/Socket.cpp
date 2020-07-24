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
        throw std::runtime_error("Couldn't create connection: new socket doesn't created");
}

Socket::~Socket()
{
	std::cout << "Close socket" << std::endl;
	CloseSocket(m_socket);
}

int Socket::Get() const
{
	return m_socket;
}