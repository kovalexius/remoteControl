#ifndef __VNC_CLIENT__H
#define __VNC_CLIENT__H

#include <string>

void update(rfbClient* cl, int x, int y, int w, int h)
{
    //SDL_Surface *sdl = static_cast<SDL_Surface*>(rfbClientGetClientData(cl, reinterpret_cast<void*>(&SDL_Init)));
}

rfbBool handleSDLEvent(rfbClient *cl, SDL_Event *e);

rfbBool resize(rfbClient* client);

void run();

class CVncClient
{
public:
	CVncClient();
	CVncClient(const std::string& _host, int _port);

	void run();
private:
    void OnMouse(const int _x, const int _y, const int _buttonMask);
    void OnMouseWheel(const int _x, const int _y, const int _wheelX, const int _wheelY);
    void OnKey(const SDL_KeyboardEvent& _key, const bool _isDown);

	void init();

	rfbClient* m_cl;
};

#endif