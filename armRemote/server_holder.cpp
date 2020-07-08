#include "server_holder.h"

std::shared_ptr<VNCServer> ServerHolder::g_vnc;

std::recursive_mutex ServerHolder::g_mutex;

std::shared_ptr<VNCServer> ServerHolder::get()
{
	auto obj = std::make_shared<DXScrCapture>();
	auto base = std::dynamic_pointer_cast<ScrCaptureBase>(obj);
	return ServerHolder::get(CRectangle(), base);
}

// Double check thread safe instanciator
std::shared_ptr<VNCServer> ServerHolder::get(const CRectangle& _region, std::shared_ptr<ScrCaptureBase>& _shooter)
{
	if (g_vnc == nullptr)
	{
		std::unique_lock<std::recursive_mutex> locker(g_mutex);
		if (g_vnc == nullptr)
		{
			g_vnc = std::make_shared<VNCServer>(_region, _shooter);
		}
	}

	return g_vnc;
}

void ServerHolder::destroy()
{
	std::unique_lock<std::recursive_mutex> locker(g_mutex);
	g_vnc.reset();
}