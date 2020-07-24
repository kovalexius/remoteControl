#include "Listener.h"

Listener::Listener(const int _port1, 
					const int _port2) : 
							m_left(_port1), 
							m_right(_port2)
{
}

void Listener::doWork()
{
	m_left.incoming();
	m_right.incoming();
}