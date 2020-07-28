#ifndef __SOCKET__H
#define __SOCKET__H

#include <string>

class Socket
{
public:
	Socket();

	// Семантика перемещения
	Socket(Socket&& _other);
	Socket& operator=(Socket&& _other);

	Socket(const int _socket);

	virtual ~Socket();

	// Копирующая семантика запрещается
	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;

	bool operator<(const Socket& _other);
	
	
	void Assign(const int _socket);
	int Get() const;
private:
	void closeSocket();

	int m_socket;
};

#endif