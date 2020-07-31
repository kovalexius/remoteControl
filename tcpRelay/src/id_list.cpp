#include "id_list.h"

#include <iostream>


void CListId::AddSocket(const std::string& _id, const Socket& _socket)
{
    std::lock_guard<std::recursive_mutex> lk(m_listMutex);
    m_listIdToSocket.insert({_id, _socket});
    m_listSocketToId.insert({_socket, _id});
}

void CListId::RemoveSocket(const Socket& _socket)
{
    std::lock_guard<std::recursive_mutex> lk(m_listMutex);
    auto itId = m_listSocketToId.find(_socket);
    if(itId != m_listSocketToId.end())
    {
        m_listIdToSocket.erase(itId->second);
        m_listSocketToId.erase(_socket);
    }
}

void CListId::RemoveSocket(const std::string& _id)
{
    std::lock_guard<std::recursive_mutex> lk(m_listMutex);
    auto itSock = m_listIdToSocket.find(_id);
    if(itSock != m_listIdToSocket.end())
    {
        m_listSocketToId.erase(itSock->second);
        m_listIdToSocket.erase(_id);
    }
}

bool CListId::CheckIdAndRemove(const std::string& _idVal, Socket& _outSock)
{
    std::lock_guard<std::recursive_mutex> lk(m_listMutex);
    std::cout << __FUNCTION__ << std::endl;
    auto itSock = m_listIdToSocket.find(_idVal);
    if(itSock != m_listIdToSocket.end())
    {
        Socket newsock(itSock->second);
        _outSock = newsock;
        m_listSocketToId.erase(itSock->second);
        m_listIdToSocket.erase(_idVal);
        //std::cout << __FUNCTION__ << " return true" << std::endl;
        return true;
    }
    //std::cout << __FUNCTION__ << " return false" << std::endl;
    return false;
}