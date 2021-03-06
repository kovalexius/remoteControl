#include "thread.h"

CThread::CThread() : m_isLaunched(false)
{}

void CThread::Run()
{
	m_isLaunched = true;
	m_thread = std::thread(&CThread::threadFunc, this);
}

void CThread::threadFunc()
{
	Worker();
}

CThread::~CThread()
{
	m_isLaunched = false;
	if(m_thread.joinable())
		m_thread.join();
}