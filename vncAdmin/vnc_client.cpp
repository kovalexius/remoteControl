#include "vnc_client.h"
#include "sdl_viewer.h"

#include <iostream>
#include <vector>
#include <signal.h>

#include "common_helpers.h"
#include "cpp_legacy_crutchs.h"


std::vector<char> CVncClient::g_tag;

CVncClient::CVncClient() : m_cl(nullptr)
{
	init();

	if (!rfbInitClient(m_cl, nullptr, nullptr))
		rfbClientLog("rfbInitClient failed");
}

CVncClient::CVncClient(const std::string& _host, int _port)
{
	init();
	//std::vector<char> host(_host.begin(), _host.end());
	//m_cl->serverHost = const_cast<char*>(_host.data());
	//m_cl->serverPort = 5900;
	//if (!rfbInitClient(m_cl, nullptr, nullptr))
	//	rfbClientLog("rfbInitClient failed");

	ConnectToRFBServer(m_cl, _host.c_str(), _port);
	//ConnectToRFBRepeater(cl, _host, _port);
}


CVncClient::~CVncClient()
{
	if (m_cl)
		cleanup(m_cl);
}

void CVncClient::update(rfbClient* _cl, int _x, int _y, int _w, int _h)
{
	CVncClient *obj = static_cast<CVncClient*>(rfbClientGetClientData(_cl, reinterpret_cast<void*>(g_tag.data())));
	if (obj)
		obj->Update(_cl, _x, _y, _w, _h);
}

rfbBool CVncClient::resize(rfbClient* _cl)
{
	CVncClient *obj = static_cast<CVncClient*>(rfbClientGetClientData(_cl, reinterpret_cast<void*>(g_tag.data())));
	if (obj)
		return obj->Resize(_cl);
	return FALSE;
}

void CVncClient::init()
{
	//rfbClientLog = rfbClientErr = log_to_file;

	const std::string tag("THIS");
	g_tag.assign(tag.begin(), tag.end());

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	atexit(SDL_Quit);
	signal(SIGINT, exit);

	/* 16-bit: cl=rfbGetClient(5,3,2); */
	m_cl = rfbGetClient(8, 3, 4);
	rfbClientSetClientData(m_cl, reinterpret_cast<void*>(g_tag.data()), this);

	m_cl->MallocFrameBuffer = CVncClient::resize;
	m_cl->canHandleNewFBSize = TRUE;
	m_cl->GotFrameBufferUpdate = CVncClient::update;
	m_cl->HandleKeyboardLedState = kbd_leds;
	m_cl->HandleTextChat = text_chat;
	m_cl->GotXCutText = got_selection;
	m_cl->GetCredential = get_credential;
	//m_cl->listenPort = LISTEN_PORT_OFFSET;
	//m_cl->listen6Port = LISTEN_PORT_OFFSET;
}

void CVncClient::Update(rfbClient* _cl, int _x, int _y, int _w, int _h)
{
	m_window.update(_x, _y, _w, _h);
}

void CVncClient::OnMouse(int _x, int _y, int _buttonMask)
{
    SendPointerEvent(m_cl, _x, _y, _buttonMask);
}

void CVncClient::OnMouseWheel(const int _x, const int _y, const int _wheelX, const int _wheelY)
{
	int steps;

	if (_wheelY > 0)
		for (steps = 0; steps < _wheelY; ++steps)
		{
			SendPointerEvent(m_cl, _x, _y, rfbButton4Mask);
			SendPointerEvent(m_cl, _x, _y, 0);
		}
	if (_wheelY < 0)
		for (steps = 0; steps > _wheelY; --steps)
		{
			SendPointerEvent(m_cl, _x, _y, rfbButton5Mask);
			SendPointerEvent(m_cl, _x, _y, 0);
		}
	if (_wheelX > 0)
		for (steps = 0; steps < _wheelX; ++steps)
		{
			SendPointerEvent(m_cl, _x, _y, BIN8(01000000));
			SendPointerEvent(m_cl, _x, _y, 0);
		}
	if (_wheelX < 0)
		for (steps = 0; steps > _wheelX; --steps)
		{
			SendPointerEvent(m_cl, _x, _y, BIN8(00100000));
			SendPointerEvent(m_cl, _x, _y, 0);
		}
}

void CVncClient::OnKey(const SDL_KeyboardEvent& _key, const bool _isDown)
{
    SendKeyEvent(m_cl,
                SDL_key2rfbKeySym(&_key),
                _isDown);
}

void CVncClient::OnTextinput(const std::string& _chr)
{
	rfbKeySym sym = utf8char2rfbKeySym(_chr);
	SendKeyEvent(m_cl, sym, TRUE);
	SendKeyEvent(m_cl, sym, FALSE);
}

void CVncClient::OnWindowExposed()
{
	SendFramebufferUpdateRequest(m_cl, 0, 0, m_cl->width, m_cl->height, FALSE);
}

void CVncClient::OnKeyboardFocus(const std::string& _text)
{
	rfbClientLog("sending clipboard text '%s'\n", _text);
	SendClientCutText(m_cl, const_cast<char*>(_text.c_str()), _text.length());
}


void CVncClient::OnWindowFocusLost(const bool _rightAltDown, const bool _leftAltDown)
{
	if (_rightAltDown)
	{
		SendKeyEvent(m_cl, XK_Alt_R, FALSE);
		rfbClientLog("released right Alt key\n");
	}
	if (_leftAltDown)
	{
		SendKeyEvent(m_cl, XK_Alt_L, FALSE);
		rfbClientLog("released left Alt key\n");
	}
}


rfbBool CVncClient::Resize(rfbClient* _client)
{
	int width = _client->width;
	int height = _client->height;
	uint8_t depth = _client->format.bitsPerPixel;
		
	_client->updateRect.x = _client->updateRect.y = 0;
	_client->updateRect.w = width; 
    _client->updateRect.h = height;

    SDL_Surface * sdlSurface = m_window.resize(_client->desktopName, width, height, depth);

	_client->width = sdlSurface->pitch / (depth / 8);
	_client->frameBuffer = static_cast<uint8_t*>(sdlSurface->pixels);
	_client->format.bitsPerPixel = depth;
	_client->format.redShift = sdlSurface->format->Rshift;
	_client->format.greenShift = sdlSurface->format->Gshift;
	_client->format.blueShift = sdlSurface->format->Bshift;
	_client->format.redMax = sdlSurface->format->Rmask >> _client->format.redShift;
	_client->format.greenMax = sdlSurface->format->Gmask >> _client->format.greenShift;
	_client->format.blueMax = sdlSurface->format->Bmask >> _client->format.blueShift;
	SetFormatAndEncodings(_client);
        
	return TRUE;
}


void CVncClient::run()
{
	while (1)
	{
		if (!m_window.DoTurn())
			break;

		int i = WaitForMessage(m_cl, 500);
		if (i<0)
		{
			return;
		}
		if (i)
			if (!HandleRFBServerMessage(m_cl))
			{
				return;
			}
	}
}