#ifndef __SERVER_HOLDER__H
#define __SERVER_HOLDER__H

#include <mutex>
#include <memory>
#include <tuple>

#include "server.h"


class ServerHolder
{
public:
	// Get existed object, it doesn't construct it. Object must be already constructed, otherwise exception
	static std::shared_ptr<VNCServer> get();
	
	//static std::shared_ptr<VNCServer> get(std::shared_ptr<ScrCaptureBase>& _shooter);

	template <class... Types>
	static std::shared_ptr<VNCServer> get(Types&&... _args)
	{
		if (g_vnc == nullptr)
		{
			std::unique_lock<std::recursive_mutex> locker(g_mutex);
			if (g_vnc == nullptr)
			{
				g_vnc = std::make_shared<VNCServer>(_args...);
			}
		}

		return g_vnc;
	}

	static void destroy();
private:


	static std::shared_ptr<VNCServer> g_vnc;
	static std::recursive_mutex g_mutex;
};

#endif