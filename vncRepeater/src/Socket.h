#ifndef __SOCKET__H
#define __SOCKET__H

#include <string>

class Socket
{
public:
	Socket();
	Socket(const int _socket);
	virtual ~Socket();

	// Копирующая семантика запрещается
	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;

	Socket& operator=(const int _socket);

	bool operator<(const Socket& _other);

	int Get() const;
private:
	void closeSocket();

	int m_socket;
};

#endif