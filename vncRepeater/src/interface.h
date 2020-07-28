#ifndef __INTERFACE__H
#define __INTERFACE__H

#include <map>
#include <string>

#include "Socket.h"
#include "id_receiver.h"

class Iface
{
public:
	Iface(const int _port);
	Socket& Listen();
	bool Accept();
private:
	CIdReceiver m_receiver;
	std::map<std::string, Socket> m_listId;
	Socket m_listenSocket;
	int m_port;
};

#endif