#ifndef __SERVER__H
#define __SERVER__H

#include <rfb/rfb.h>

#include "types/geometry_types.h"

#include "screenshot_poly.h"


class VNCServer
{
public:
	VNCServer(const CRectangle& _region, std::shared_ptr<ScrCaptureBase>& _shooter);
	virtual ~VNCServer();

	void run();
private:
	void initBuffer(unsigned char* buffer);
	void newframebuffer(rfbScreenInfoPtr screen, int width, int height);

	/* Here the pointer events are handled */
	static void doptr(int buttonMask, int x, int y, rfbClientPtr cl);
	static void doDisplayHook(struct _rfbClientRec* cl);
	static enum rfbNewClientAction VNCServer::newclient(rfbClientPtr cl);

	// Переделать на полиморфизм
	std::shared_ptr<ScrCaptureBase> m_shooter;

	CRectangle	m_region;

	rfbScreenInfoPtr m_rfbScreen;
};

#endif