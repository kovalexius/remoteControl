#ifndef __PREBUF__H
#define __PREBUF__H

#include <map>
#include <string>
#include <mutex>

#include "Socket.h"


class CMsgList
{
public:
    void PushMessage(const Socket& _socket, const std::string& _msg);
    bool PopMessage(const Socket& _socket, std::string& _msg);

private:
    std::recursive_mutex m_mutex;
    std::map<Socket, std::string> m_listBuffers;
};

#endif