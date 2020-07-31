#ifndef __LISTENER__H
#define __LISTENER__H

#include <map>
#include <string>

#include "interface.h"
#include "Socket.h"

class Listener
{
public:
	Listener(const int _port1, const int _port2);

	void doWork();
protected:
	Iface m_left;
	Iface m_right;
};

#endif