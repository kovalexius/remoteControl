#ifndef __INTERFACE__H
#define __INTERFACE__H

#include <map>
#include <string>
#include <memory>
#include <mutex>

#include "Socket.h"
#include "multiplexer.h"
#include "id_list.h"

class Iface
{
public:
	Iface(const int _port);
	Socket& Listen(Iface* _neighbor);
	bool Accept();
    bool CheckId(const std::string& _idVal, Socket& _outSock);
private:
    void onRead(Socket _socket);
    void onHup(const Socket& _socket);

    Iface *m_neighbor;
	CMultiplexer m_multiplexer;
	CListId     m_listId;
	Socket m_listenSocket;
	int m_port;
};

void passThroughThread(const Socket _sock1, const Socket _sock2);

#endif