#ifndef __SOCKET__H
#define __SOCKET__H

#include <string>

class Socket
{
public:
	Socket();
	virtual ~Socket();

	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;

	int Get() const;
private:
	int m_socket;
};

#endif