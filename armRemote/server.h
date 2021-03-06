#ifndef __SERVER__H
#define __SERVER__H

#include <memory>
#include <cstring>
#include <exception>

#include <rfb/rfb.h>

#include "types/geometry_types.h"
#include "screenshot_poly.h"


class VNCServer
{
public:
	VNCServer(std::shared_ptr<ScrCaptureBase>& _shooter);
	virtual ~VNCServer();

	void Listen();
    void Connect(const std::string& _host,
                    int _port);
    void Connect(const std::string& _repeaterHost, 
                    int _repeaterPort,
		            const std::string& _idConnect);

	void Run();
private:
    void init();
	void initBuffer(unsigned char* buffer);
	void newframebuffer(rfbScreenInfoPtr screen, int width, int height);

	/* Here the pointer events are handled */
	static void doptr(int buttonMask, int x, int y, rfbClientPtr cl);
	static void doDisplayHook(struct _rfbClientRec* cl);
	static enum rfbNewClientAction newclient(rfbClientPtr cl);

	std::shared_ptr<ScrCaptureBase> m_shooter;

	CRectangle	m_region;

	rfbScreenInfoPtr m_rfbScreen;
};

#endif