#ifndef __SDL_VNCVIEWER__H
#define __SDL_VNCVIEWER__H

#include <rfb/rfbclient.h>
#include <SDL.h>

void update(rfbClient* cl, int x, int y, int w, int h);

rfbBool handleSDLEvent(rfbClient *cl, SDL_Event *e);

rfbBool resize(rfbClient* client);

void run();

class CSdlVncViewer
{
public:
	CSdlVncViewer();
	~CSdlVncViewer();
private:

};

#endif