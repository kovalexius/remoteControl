#include "interface.h"

#include "common.h"

// https://habr.com/ru/company/infopulse/blog/415259/


Iface::Iface(const int _port) : m_port(_port)
{
}

Socket& Iface::incoming()
{
	bind_socket(m_listenSocket, std::string(), m_port);
	listen_socket(m_listenSocket);

	return m_listenSocket;
}