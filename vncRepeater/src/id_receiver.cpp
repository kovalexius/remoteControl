#include "id_receiver.h"

void CIdReceiver::Iteration()
{
	m_efd = epoll_create(1);
}

void CIdReceiver::Iteration()
{
	
	char buf[MESSAGE_MAX_LEN];
	std::string msg;
    auto recv_size = read(dataSocket.Get(), buf, MESSAGE_MAX_LEN);
    if(recv_size > 0)
    msg.insert(0, buf, recv_size);

}