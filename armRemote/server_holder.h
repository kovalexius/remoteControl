#ifndef __SERVER_HOLDER__H
#define __SERVER_HOLDER__H

#include <mutex>
#include <memory>

#include "server.h"

class ServerHolder
{
public:
	// Double check thread safe instanciator
	static std::shared_ptr<VNCServer> get();
	static void destroy();
private:
	static std::shared_ptr<VNCServer> g_vnc;
	static std::recursive_mutex g_mutex;
};

#endif