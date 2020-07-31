#ifndef __ID_LIST__H
#define __ID_LIST__H

#include <map>
#include <string>
#include <mutex>

#include "Socket.h"

class CListId
{
public:
    void AddSocket(const std::string& _id, const Socket& _socket);
    void RemoveSocket(const Socket& _socket);
    void RemoveSocket(const std::string& _id);
    bool CheckIdAndRemove(const std::string& _idVal, Socket& _outSock);

private:
    std::recursive_mutex  m_listMutex;
    std::map<std::string, Socket> m_listIdToSocket;
    std::map<Socket, std::string> m_listSocketToId;
};

#endif