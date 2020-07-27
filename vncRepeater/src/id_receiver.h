#ifndef __ID_RECEIVER__H
#define __ID_RECEIVER__H

#include "thread.h"
#include "Socket.h"

class CIdReceiver : public CThread
{
public:
	void Iteration() override;
	void AddSocket(const Socket& _socket);
private:
	// epoll descriptor for signaling data receive
    int m_efd;
};

#endif