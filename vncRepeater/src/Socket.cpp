#include "Socket.h"

#include <stdexcept>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>

#ifdef WIN32
#define CloseSocket closesocket
#else
#define CloseSocket close
#endif

constexpr int LISTEN_BACKLOG = 1024;

int create_tcp_socket()
{
    int sfd;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Couldn't create connection: new socket doesn't created");
    
    return sfd;
}

void bind_to_interface(const int sfd,
						const std::string& iface_addr,
                        const int iface_port)
{
    sockaddr_in bind_addr;
    memset(&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;

	if(iface_addr.empty())
		bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		bind_addr.sin_addr.s_addr = inet_addr(iface_addr.c_str());
    
	bind_addr.sin_port = htons(iface_port);
    
	if(bind( sfd, (struct sockaddr*)&bind_addr, sizeof(bind_addr) ) == -1)
    {
        CloseSocket(sfd);
        throw std::runtime_error(strerror(errno));
    }
}

Socket::Socket() : m_socket(create_tcp_socket())
{
}

Socket::Socket(const int _port) : m_socket(create_tcp_socket())
{
	bind_to_interface(m_socket, std::string(), _port);
}

Socket::Socket(const std::string& _address, const int _port) : m_socket(create_tcp_socket())
{
	bind_to_interface(m_socket, _address, _port);
}

Socket::~Socket()
{
	CloseSocket(m_socket);
}

int Socket::Get()
{
	return m_socket;
}