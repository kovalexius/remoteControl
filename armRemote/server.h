#ifndef __SERVER__H
#define __SERVER__H

#include "types/geometry_types.h"
#include "GDIScreenShooter.h"

#include <rfb/rfb.h>

class VNCServer
{
public:
	VNCServer() = delete;
	explicit VNCServer(const CRectangle& _region);
private:
	void initBuffer(unsigned char* buffer);
	void newframebuffer(rfbScreenInfoPtr screen, int width, int height);

	/* Here the pointer events are handled */
	static void VNCServer::doptr(int buttonMask, int x, int y, rfbClientPtr cl);

	static enum rfbNewClientAction VNCServer::newclient(rfbClientPtr cl);

	// Переделать на полиморфизм
	CGDIScreenShooter m_shooter;

	CRectangle	m_region;

	rfbScreenInfoPtr m_rfbScreen;
};

#endif