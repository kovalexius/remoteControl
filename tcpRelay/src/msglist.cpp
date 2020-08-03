#include "msglist.h"

void CMsgList::PushMessage(const Socket& _socket, const std::string& _msg)
{
    std::lock_guard<std::recursive_mutex> lk(m_mutex);
    auto it = m_listBuffers.find(_socket);
    if(it != m_listBuffers.end())
        it->second += _msg;
    else
        m_listBuffers.insert({_socket, _msg});
}

bool CMsgList::PopMessage(const Socket& _socket, std::string& _msg)
{
    std::lock_guard<std::recursive_mutex> lk(m_mutex);
    auto it = m_listBuffers.find(_socket);
    if(it != m_listBuffers.end())
    {
        _msg = it->second;
        m_listBuffers.erase(_socket);
        return true;
    }
    return false;
}