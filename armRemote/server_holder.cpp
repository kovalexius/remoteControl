#include "server_holder.h"

std::shared_ptr<VNCServer> ServerHolder::g_vnc;

std::recursive_mutex ServerHolder::g_mutex;


std::shared_ptr<VNCServer> ServerHolder::get()
{
	if(g_vnc == nullptr)
		throw std::string("Singletone not initialized! Please, call get() with args!");
	return g_vnc;
}

/*
// Double check thread safe instanciator
std::shared_ptr<VNCServer> ServerHolder::get(std::shared_ptr<ScrCaptureBase>& _shooter)
{
	if (g_vnc == nullptr)
	{
		std::unique_lock<std::recursive_mutex> locker(g_mutex);
		if (g_vnc == nullptr)
		{
			g_vnc = std::make_shared<VNCServer>(_shooter);
		}
	}

	return g_vnc;
}
*/

void ServerHolder::destroy()
{
	std::unique_lock<std::recursive_mutex> locker(g_mutex);
	g_vnc.reset();
}