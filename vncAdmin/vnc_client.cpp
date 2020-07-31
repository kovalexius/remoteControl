#include "vnc_client.h"
#include "sdl_viewer.h"

#include <iostream>
#include <vector>
#include <functional>
#include <signal.h>

#include "common_helpers.h"
#include "cpp_legacy_crutchs.h"


std::vector<char> CVncClient::g_tag;

CVncClient::CVncClient() : m_cl(nullptr)
{
}

void CVncClient::Connect()
{
	m_cl = rfbGetClient(8, 3, 4);
	initCallbacks(m_cl);

	if (!rfbInitClient(m_cl, nullptr, nullptr))
		rfbClientLog("rfbInitClient failed");
}

void CVncClient::Connect(const std::string& _host, int _port)
{
	m_cl = rfbGetClient(8, 3, 4);
	initCallbacks(m_cl);

	if (!ConnectToRFBServer(m_cl, _host.c_str(), _port))
		std::cout << "failed ConnectToRFBServer" << std::endl;

	initConnection(m_cl);
}

void CVncClient::Connect(const std::string& _repeaterHost, const int _repeaterPort, const std::string& _desthost, const int _destport)
{
	m_cl = rfbGetClient(8, 3, 4);
	initCallbacks(m_cl);

	if (!ConnectToRFBRepeater(m_cl, _repeaterHost.c_str(), _repeaterPort, _desthost.c_str(), _destport))
		std::cout << "failed ConnectToRFBRepeater" << std::endl;

	initConnection(m_cl);
}

void CVncClient::Connect(const std::string& _repeaterHost, const int _repeaterPort, const std::string& _idConnect)
{
	m_cl = rfbGetClient(8, 3, 4);
	initCallbacks(m_cl);

	if (!ConnectToRFBServer(m_cl, _repeaterHost.c_str(), _repeaterPort))
		std::cout << "failed ConnectToRFBServer" << std::endl;

	char buf[250];
	memset(buf, 0, sizeof(buf));
	if (snprintf(buf, sizeof(buf), "ID:%s", _idConnect.c_str()) >= (int)sizeof(buf))
	{
		std::cout << "Error, given ID is too long." << std::endl;
	}
	
	if (!WriteToRFBServer(m_cl, buf, sizeof(buf)))
		std::cout << "Failed to WriteToRFBServer" << std::endl;

	initConnection(m_cl);
}


void CVncClient::initConnection(rfbClient* _cl)
{
	if (!InitialiseRFBConnection(m_cl))
	{
		std::cout << "failed InitialiseRFBConnection" << std::endl;
		//throw std::runtime_error("failed InitialiseRFBConnection");
	}

	//*
	m_cl->width = m_cl->si.framebufferWidth;
	m_cl->height = m_cl->si.framebufferHeight;
	if (!m_cl->MallocFrameBuffer(m_cl))
		std::cout << "failed MallocFrameBuffer" << std::endl;
	if (!SetFormatAndEncodings(m_cl))
		std::cout << "failed SetFormatAndEncodings" << std::endl;
	/**/

	if (!SendFramebufferUpdateRequest(m_cl,
		m_cl->updateRect.x, m_cl->updateRect.y,
		m_cl->updateRect.w, m_cl->updateRect.h, FALSE))
		std::cout << "failed SendFramebufferUpdateRequest" << std::endl;
}

void CVncClient::initCallbacks(rfbClient* _cl)
{
	//rfbClientLog = rfbClientErr = log_to_file;

	const std::string tag("THIS");
	g_tag.assign(tag.begin(), tag.end());

	/* place 'this' to client data */
	rfbClientSetClientData(_cl, reinterpret_cast<void*>(g_tag.data()), this);

	_cl->MallocFrameBuffer = CVncClient::resize;
	_cl->canHandleNewFBSize = TRUE;
	_cl->GotFrameBufferUpdate = CVncClient::update;
	_cl->HandleKeyboardLedState = kbd_leds;
	//_cl->HandleTextChat = text_chat;
	_cl->GotXCutText = got_selection;
	_cl->GetCredential = get_credential;
	_cl->listenPort = LISTEN_PORT_OFFSET;
	_cl->listen6Port = LISTEN_PORT_OFFSET;

	//m_window.resize("AdminKit", 300, 300, 8);
	m_window.SubscribeOnSdlKey(std::bind(&CVncClient::OnKey, this, std::placeholders::_1, std::placeholders::_2));
	m_window.SubscribeOnSdlMouse(std::bind(&CVncClient::OnMouse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_window.SubscribeOnSdlMouseWheel(std::bind(&CVncClient::OnMouseWheel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_window.SubscribemOnSdlWindowFocusLost(std::bind(&CVncClient::OnWindowFocusLost, this, std::placeholders::_1, std::placeholders::_2));
}

CVncClient::~CVncClient()
{
	if (m_cl)
	{
		rfbClientCleanup(m_cl);
		//cleanup(m_cl);
	}
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

void CVncClient::Update(rfbClient* _cl, int _x, int _y, int _w, int _h)
{
	m_window.update(_x, _y, _w, _h);
}

rfbBool CVncClient::Resize(rfbClient* _client)
{
	int width = _client->width;
	int height = _client->height;
	uint8_t depth = _client->format.bitsPerPixel;

	std::string desktopName; 
	if (_client->desktopName)
		desktopName.assign(_client->desktopName);
	SDL_Surface * sdlSurface = m_window.resize(desktopName, width, height, depth);

	_client->updateRect.x = _client->updateRect.y = 0;
	_client->updateRect.w = width;
	_client->updateRect.h = height;

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
	SendClientCutText(m_cl, strdup(_text.c_str()), _text.length());
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


void CVncClient::Run()
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