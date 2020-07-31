#ifndef __SOCKET__H
#define __SOCKET__H

#include <string>
#include <memory>

class SocketImpl;
class Socket
{
public:
	Socket();

    // Копирующая семантика
	Socket(const Socket&);
	Socket& operator=(const Socket&);

	// Семантика перемещения запрещается
	Socket(Socket&& _other);
	Socket& operator=(Socket&& _other);

	Socket(const int _socket);

	virtual ~Socket();

	bool operator<(const Socket& _other) const;
	
	
	void Assign(const int _socket);
	int Get() const;
private:
	std::shared_ptr<SocketImpl> m_socket;
};

#endif