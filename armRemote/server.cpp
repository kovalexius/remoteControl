#include "server.h"
#include "server_holder.h"


VNCServer::VNCServer(const CRectangle& _region, std::shared_ptr<ScrCaptureBase>& _shooter) :
													m_region(_region),
													m_shooter(_shooter)
{
	m_rfbScreen = rfbGetScreen(nullptr,
								nullptr, 
								m_region.getSize().m_x, 
								m_region.getSize().m_y,
								8, 
								3, 
								m_region.getBytesPerPixel());
	if (!m_rfbScreen)
		throw std::exception("Can't initialize rfbScreen");
	m_rfbScreen->frameBuffer = nullptr;
	m_rfbScreen->frameBuffer = static_cast<char*>(malloc(m_region.getSize().m_x * m_region.getSize().m_y * m_region.getBytesPerPixel()));
	initBuffer((unsigned char*)m_rfbScreen->frameBuffer);
	m_rfbScreen->desktopName = "ARM";
	m_rfbScreen->alwaysShared = TRUE;
	//m_rfbScreen->ptrAddEvent = doptr;
	//m_rfbScreen->kbdAddEvent;
	m_rfbScreen->displayHook = doDisplayHook;
}

VNCServer::~VNCServer()
{
	if (!m_rfbScreen)
		return;

	if (m_rfbScreen->frameBuffer)
		free(m_rfbScreen->frameBuffer);

	rfbScreenCleanup(m_rfbScreen);
}

void VNCServer::run()
{
	rfbInitServer(m_rfbScreen);
	//rfbRunEventLoop(m_rfbScreen, 4000, FALSE);
	//rfbRunEventLoop(m_rfbScreen, 4000, TRUE); //background mode. It needs pthread library, but I haven't it on windows

	while (rfbIsActive(m_rfbScreen))
	{
		std::vector<char> buffer;
		if (!m_shooter->GetScrCapture(m_region, buffer))
			return;
		std::memcpy(m_rfbScreen->frameBuffer, buffer.data(), buffer.size());
		rfbMarkRectAsModified(m_rfbScreen, 0, 0, m_region.getSize().m_x, m_region.getSize().m_y);

		rfbProcessEvents(m_rfbScreen, 10000);
	}
}

void VNCServer::initBuffer(unsigned char* buffer)
{
	int i, j;
	for (j = 0; j < m_region.getSize().m_y; ++j) 
	{
		for (i = 0; i < m_region.getSize().m_x; ++i)
		{
			buffer[(j * m_region.getSize().m_x + i) * m_region.getBytesPerPixel() + 0] = 
				(i + j) * 128 / (m_region.getSize().m_x + m_region.getSize().m_y);		/* red */
			buffer[(j * m_region.getSize().m_x + i) * m_region.getBytesPerPixel() + 1] =
				i * 128 / m_region.getSize().m_x;					/* green */
			buffer[(j * m_region.getSize().m_x + i) * m_region.getBytesPerPixel() + 2] =
				j * 256 / m_region.getSize().m_y;					/* blue */
		}
		buffer[j * m_region.getSize().m_x * m_region.getBytesPerPixel() + 0] = 0xff;
		buffer[j * m_region.getSize().m_x * m_region.getBytesPerPixel() + 1] = 0xff;
		buffer[j * m_region.getSize().m_x * m_region.getBytesPerPixel() + 2] = 0xff;
		buffer[j * m_region.getSize().m_x * m_region.getBytesPerPixel() + 3] = 0xff;
	}
}

void VNCServer::newframebuffer(rfbScreenInfoPtr screen, int width, int height)
{
	unsigned char *oldfb, *newfb;

	oldfb = (unsigned char*)screen->frameBuffer;
	newfb = (unsigned char*)malloc(m_region.getSize().m_x * m_region.getSize().m_y * m_region.getBytesPerPixel());
	initBuffer(newfb);
	rfbNewFramebuffer(screen, (char*)newfb, m_region.getSize().m_x, m_region.getSize().m_y, 8, 3, m_region.getBytesPerPixel());
	free(oldfb);

	/*** FIXME: Re-install cursor. ***/
}

/* Here the pointer events are handled */
void VNCServer::doptr(int buttonMask, int x, int y, _rfbClientRec* cl)
{
	std::vector<char> buffer;
	auto obj = ServerHolder::get();
	if (!obj->m_shooter->GetScrCapture(obj->m_region, buffer))
		return;

	std::memcpy(cl->screen->frameBuffer, buffer.data(), buffer.size());

	rfbMarkRectAsModified(cl->screen, 0, 0, obj->m_region.getSize().m_x, obj->m_region.getSize().m_y);
	rfbDefaultPtrAddEvent(buttonMask, x, y, cl);
}

enum rfbNewClientAction VNCServer::newclient(_rfbClientRec* cl)
{
	//cl->clientData = (void*)calloc(sizeof(ClientData), 1);
	//cl->clientGoneHook = clientgone;
	return RFB_CLIENT_ACCEPT;
}

void VNCServer::doDisplayHook(struct _rfbClientRec* cl)
{
	std::vector<char> buffer;
	auto obj = ServerHolder::get();
	if (!obj->m_shooter->GetScrCapture(obj->m_region, buffer))
		return;

	//cl->screen->frameBuffer = buffer.data();
	std::memcpy(cl->screen->frameBuffer, buffer.data(), buffer.size());

	rfbMarkRectAsModified(cl->screen, 0, 0, obj->m_region.getSize().m_x, obj->m_region.getSize().m_y);
}