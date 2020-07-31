#ifndef __SOCKET_IMPL__H
#define __SOCKET_IMPL__H

class SocketImpl
{
public:
	SocketImpl();
    SocketImpl(const int _socket);

    // Копирующая семантика запрещается
	SocketImpl(const SocketImpl&) = delete;
	SocketImpl& operator=(const SocketImpl&) = delete;

	// Семантика перемещения запрещается
	SocketImpl(SocketImpl&& _other) = delete;
	SocketImpl& operator=(SocketImpl&& _other) = delete;

	virtual ~SocketImpl();
	
	void Assign(const int _socket);
	int Get() const;
private:
	void closeSocket();

	int m_socket;
};

#endif