#include "server.h"



VNCServer::VNCServer(const CRectangle& _region) : m_region(_region)
{
	m_rfbScreen = rfbGetScreen(nullptr,
								nullptr, 
								m_region.m_size.m_x, 
								m_region.m_size.m_y,
								8, 
								3, 
								m_region.m_bytespp);
	if (!m_rfbScreen)
		throw std::exception("Can't initialize rfbScreen");
	m_rfbScreen->frameBuffer = static_cast<char*>(malloc(m_region.m_size.m_x * m_region.m_size.m_y * m_region.m_bytespp));
	//screen->frameBuffer = nullptr;
	//initBuffer((unsigned char*)screen->frameBuffer);
	m_rfbScreen->desktopName = "ARM";
	m_rfbScreen->alwaysShared = TRUE;
	m_rfbScreen->ptrAddEvent = doptr;
	m_rfbScreen->kbdAddEvent;
}

void VNCServer::initBuffer(unsigned char* buffer)
{
	int i, j;
	for (j = 0; j < m_region.m_size.m_y; ++j) 
	{
		for (i = 0; i < m_region.m_size.m_x; ++i) 
		{
			buffer[(j * (m_region.m_size.m_x + i) * m_region.m_bytespp + 0] = (i + j) * 128 / (m_region.m_size.m_x + m_region.m_size.m_y);		/* red */
			buffer[(j * (m_region.m_size.m_x + i) * m_region.m_bytespp + 1] = i * 128 / m_region.m_size.m_x;					/* green */
			buffer[(j * (m_region.m_size.m_x + i) * m_region.m_bytespp + 2] = j * 256 / m_region.m_size.m_y;					/* blue */
		}
		buffer[j * m_region.m_size.m_x * m_region.m_bytespp + 0] = 0xff;
		buffer[j * m_region.m_size.m_x * m_region.m_bytespp + 1] = 0xff;
		buffer[j * m_region.m_size.m_x * m_region.m_bytespp + 2] = 0xff;
		buffer[j * m_region.m_size.m_x * m_region.m_bytespp + 3] = 0xff;
	}
}

void VNCServer::newframebuffer(rfbScreenInfoPtr screen, int width, int height)
{
	unsigned char *oldfb, *newfb;

	oldfb = (unsigned char*)screen->frameBuffer;
	newfb = (unsigned char*)malloc(m_region.m_size.m_x * m_region.m_size.m_y * m_region.m_bytespp);
	initBuffer(newfb);
	rfbNewFramebuffer(screen, (char*)newfb, m_region.m_size.m_x, m_region.m_size.m_y, 8, 3, m_region.m_bytespp);
	free(oldfb);

	/*** FIXME: Re-install cursor. ***/
}

/* Here the pointer events are handled */
void VNCServer::doptr(int buttonMask, int x, int y, _rfbClientRec* cl)
{
	BITMAPINFOHEADER infoheader;
	std::vector<char> buffer;
	if (!m_shooter.getScreenshot(m_region, buffer))
		return;

	//cl->screen->frameBuffer = buffer.data();
	std::memcpy(cl->screen->frameBuffer, buffer.data(), buffer.size());

	rfbMarkRectAsModified(cl->screen, 0, 0, WIDTH, HEIGHT);
	rfbDefaultPtrAddEvent(buttonMask, x, y, cl);
}

enum rfbNewClientAction VNCServer::newclient(_rfbClientRec* cl)
{
	//cl->clientData = (void*)calloc(sizeof(ClientData), 1);
	//cl->clientGoneHook = clientgone;
	return RFB_CLIENT_ACCEPT;
}