#ifndef __SOCKET__H
#define __SOCKET__H

#include <string>

class Socket
{
public:
	Socket();
	Socket(const int _port);
	Socket(const std::string& _address, const int _port);
	virtual ~Socket();

	int Get();
private:
	int m_socket;
};

#endif