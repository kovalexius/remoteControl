#include "server.h"
#include "server_holder.h"

#include <iostream>


VNCServer::VNCServer(std::shared_ptr<ScrCaptureBase>& _shooter) :
							m_shooter(_shooter)
{
	init();
	rfbInitServer(m_rfbScreen);
}

VNCServer::VNCServer(std::shared_ptr<ScrCaptureBase>& _shooter,
			const std::string& _host,
			int _port) : 
							m_shooter(_shooter)
{
	init();
	rfbInitSockets(m_rfbScreen);
	rfbHttpInitSockets(m_rfbScreen);
	rfbConnect(m_rfbScreen, strdup(_host.c_str()), _port);
}


static void clientGone(rfbClientPtr cl)
{
	std::cout << std::endl << "Client Gone" << std::endl;
	//rfbShutdownServer(cl->screen, TRUE);
}

VNCServer::VNCServer(std::shared_ptr<ScrCaptureBase>& _shooter,
	const std::string& _host, int _port,
	const std::string& _idConnect) :
									m_shooter(_shooter)
{
	init();
	m_rfbScreen->port = -1;
	m_rfbScreen->ipv6port = -1;
	rfbInitSockets(m_rfbScreen);
	//rfbHttpInitSockets(m_rfbScreen);
	//auto sock = rfbConnect(m_rfbScreen, strdup(_host.c_str()), _port);
	auto sock = rfbConnectToTcpAddr(strdup(_host.c_str()), _port);
	if (sock == RFB_INVALID_SOCKET)
	{
		std::cout << "failed rfbConnectToTcpAddr" << std::endl;
	}

	char buf[250];
	memset(buf, 0, sizeof(buf));
	if (snprintf(buf, sizeof(buf), "ID:%s", _idConnect.c_str()) >= (int)sizeof(buf))
	{
		std::cout << "Error, given ID is too long." << std::endl;
	}
	if (send(sock, buf, sizeof(buf), 0) != sizeof(buf))
	{
		std::cout << "Failed to send 'ID'" << std::endl;
	}

	//*
	rfbClientPtr cl = rfbNewClient(m_rfbScreen, sock);
	if (!cl)
	{
		std::cout << "failed rfbNewClient" << std::endl;
	}
	cl->reverseConnection = 0;
	cl->clientGoneHook = clientGone;
	/**/

	//rfbInitServer(m_rfbScreen);
}

VNCServer::~VNCServer()
{
	if (!m_rfbScreen)
		return;

	if (m_rfbScreen->frameBuffer)
		free(m_rfbScreen->frameBuffer);

	rfbScreenCleanup(m_rfbScreen);
}

void VNCServer::init()
{
	m_region = m_shooter->getRectangle();

	m_rfbScreen = rfbGetScreen(nullptr,
								nullptr, 
								m_region.getSize().m_x, 
								m_region.getSize().m_y,
								8, 
								3, 
								m_region.getBytesPerPixel());
	if (!m_rfbScreen)
		throw std::string("Can't initialize rfbScreen");
	m_rfbScreen->frameBuffer = nullptr;
	m_rfbScreen->frameBuffer = static_cast<char*>(malloc(m_region.getSize().m_x * m_region.getSize().m_y * m_region.getBytesPerPixel()));
	initBuffer((unsigned char*)m_rfbScreen->frameBuffer);
	m_rfbScreen->desktopName = "ARM";
	m_rfbScreen->alwaysShared = TRUE;
	//m_rfbScreen->ptrAddEvent = doptr;
	//m_rfbScreen->kbdAddEvent;
	//m_rfbScreen->displayHook = doDisplayHook;
}

void VNCServer::run()
{
	
	//rfbRunEventLoop(m_rfbScreen, 4000, FALSE);
	//rfbRunEventLoop(m_rfbScreen, 4000, TRUE); //background mode. It needs pthread library, but I haven't it on windows

	CRectangle region; 
	while (rfbIsActive(m_rfbScreen))
	{
		std::vector<char> buffer;
		if (!m_shooter->GetScrCapture(region, buffer))
			return;

		if(region != m_region)
		{
			std::cout << "region != m_region" << std::endl;
			m_region = region;
			unsigned char* oldfb = (unsigned char*)m_rfbScreen->frameBuffer;
			unsigned char* newfb = (unsigned char*)malloc(m_region.getSize().m_x * m_region.getSize().m_y * m_region.getBytesPerPixel());
			initBuffer(newfb);
			rfbNewFramebuffer(m_rfbScreen, (char*)newfb, m_region.getSize().m_x, m_region.getSize().m_y, 8, 3, m_region.getBytesPerPixel());
			free(oldfb);
		}

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