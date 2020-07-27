#ifndef __THREAD__H
#define __THREAD__H

#include <thread>

class CThread
{
public:
	CThread();
	~CThread();

	void Run();
	virtual void Iteration()
	{}
private:
	void threadFunc();
	bool m_isLaunched;
	std::thread m_thread;
};

#endif