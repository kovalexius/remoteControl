#ifndef __MULTIPLEXER__H
#define __MULTIPLEXER__H

#include <set>
#include <map>
#include <thread>
#include <mutex>

#include "thread.h"
#include "Socket.h"

class CMultiplexer : public CThread
{
public:
    using OnReceive = std::function<void(const Socket&)>;
    using OnHup = std::function<void (const Socket& _socket)>;

	CMultiplexer();
	virtual ~CMultiplexer();

	bool AddSocket(Socket& _socket);
    bool RemoveSocket(const Socket& _socket);
    void SubscribeOnReceive(OnReceive _onReceive);
    void SubscribeOnHup(OnHup _onHup);
private:
    void Worker() override;

	void process_hup_events(const struct epoll_event &_evnt);
	void process_input_events(const struct epoll_event &_evnt);

    OnReceive   m_receive_handle;
    OnHup       m_onHup_handle;

	// epoll descriptor for signaling data receive
    int m_efd;

	std::map<int, Socket> m_socks;
    std::recursive_mutex m_mutex;
};

#endif