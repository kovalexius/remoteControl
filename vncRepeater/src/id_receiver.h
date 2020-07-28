#ifndef __ID_RECEIVER__H
#define __ID_RECEIVER__H

#include <set>
#include <map>
#include <thread>
#include <mutex>

#include "thread.h"
#include "Socket.h"

class CIdReceiver : public CThread
{
public:
	CIdReceiver();
	~CIdReceiver();

	void Worker() override;
	bool AddSocket(Socket& _socket);
private:
	void process_hup_events(const struct epoll_event &evnt);

	// epoll descriptor for signaling data receive
    int m_efd;

	std::map<int, Socket> m_socks;
    std::mutex m_mutex;
};

#endif