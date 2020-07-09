#ifndef __SERVER_HOLDER__H
#define __SERVER_HOLDER__H

#include <mutex>
#include <memory>

#include "server.h"


class ServerHolder
{
public:
	// Get existed object, it doesn't construct it. Object must be already constructed, otherwise exception
	static std::shared_ptr<VNCServer> get();
	
	static std::shared_ptr<VNCServer> get(const CRectangle& _region, std::shared_ptr<ScrCaptureBase>& _shooter);
	static void destroy();
private:
	static std::shared_ptr<VNCServer> g_vnc;
	static std::recursive_mutex g_mutex;
};

#endif