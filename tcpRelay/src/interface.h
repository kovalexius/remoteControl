#ifndef __INTERFACE__H
#define __INTERFACE__H

#include <map>
#include <string>
#include <memory>
#include <mutex>

#include "Socket.h"
#include "multiplexer.h"
#include "id_list.h"
#include "msglist.h"

struct Prebuffer
{
    Prebuffer() : m_socket(-1)
    {}
    Socket m_socket;
    std::string m_buffer;
};

class Iface
{
public:
	Iface(const int _port);
	Socket& Listen(Iface* _neighbor);
	bool Accept();
    bool CheckIdRemove(const std::string& _idVal, Prebuffer& _prebufer);
private:
    void processId(const std::string& _id, const Socket& _socket);
    void onRead(const Socket& _socket);
    void onHup(const Socket& _socket);

    Iface *m_neighbor;
	CMultiplexer m_multiplexer;
	CListId     m_listId;
    CMsgList    m_listMsg;
	Socket m_listenSocket;
	int m_port;
};

void passThroughThread(const Prebuffer _prebuffer1, const Prebuffer _prebuffer2);

#endif