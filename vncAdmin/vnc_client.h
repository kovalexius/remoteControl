#ifndef __VNC_CLIENT__H
#define __VNC_CLIENT__H

#include <string>
#include <vector>

#include <rfb/rfbclient.h>
#include <SDL.h>

#include "sdl_viewer.h"

class CVncClient
{
public:
	CVncClient();
	CVncClient(const std::string& _host, const int _port);
	CVncClient(const std::string& _repeaterHost, const int _repeaterPort, const std::string& _desthost, const int _destport);
	CVncClient(const std::string& _repeaterHost, const int _repeaterPort, const std::string& _idConnect);

	virtual ~CVncClient();

	void run();

private:
	void Update(rfbClient* _cl, int _x, int _y, int _w, int _h);
	rfbBool Resize(rfbClient* _client);

	static void update(rfbClient* _cl, int _x, int _y, int _w, int _h);
	static rfbBool resize(rfbClient* _client);

    void OnMouse(const int _x, const int _y, const int _buttonMask);
    void OnMouseWheel(const int _x, const int _y, const int _wheelX, const int _wheelY);
    void OnKey(const SDL_KeyboardEvent& _key, const bool _isDown);
	void OnTextinput(const std::string& _chr);
	void OnWindowExposed();
	void OnKeyboardFocus(const std::string& _text);
	void OnWindowFocusLost(const bool _rightAltDown, const bool _leftAltDown);

	void initCallbacks(rfbClient* _cl);
	void initConnection(rfbClient* _cl);

	rfbClient* m_cl;
	CSdlViewer m_window;

	static std::vector<char> g_tag;
};

#endif