#ifndef __THREAD__H
#define __THREAD__H

#include <thread>

class CThread
{
public:
	CThread();
	~CThread();

	void Run();
	virtual void Worker()
	{}
	
protected:
	bool m_isLaunched;

private:
	void threadFunc();
	std::thread m_thread;
};

#endif