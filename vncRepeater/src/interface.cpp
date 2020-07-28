#include "interface.h"

#include <iostream>

#include <string.h>

#include "common.h"


// https://habr.com/ru/company/infopulse/blog/415259/

Iface::Iface(const int _port) : m_port(_port)
{}

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

	m_receiver.AddSocket(dataSocket);
}
