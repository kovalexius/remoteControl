#include "server_holder.h"

std::shared_ptr<VNCServer> ServerHolder::g_vnc;

std::recursive_mutex ServerHolder::g_mutex;

std::shared_ptr<VNCServer> ServerHolder::get()
{
	if (g_vnc == nullptr)
	{
		std::unique_lock<std::recursive_mutex> locker(g_mutex);
		if (g_vnc == nullptr)
		{
			g_vnc = std::make_shared<VNCServer>();
		}
	}

	return g_vnc;
}

void ServerHolder::destroy()
{
	std::unique_lock<std::recursive_mutex> locker(g_mutex);
	g_vnc.reset();
}