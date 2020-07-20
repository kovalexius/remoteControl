#include "sdl_client.h"
#include "SDLvncviewer.h"

#include <signal.h>

#include "common_helpers.h"

CVncClient::CVncClient()
{
	init();

	if (!rfbInitClient(cl, nullptr, nullptr))
	{
		cleanup(cl);
		cl = NULL; /* rfbInitClient has already freed the client struct */

		return;
	}
}

CVncClient::CVncClient(const std::string& _host, int _port)
{
	init();

	//cl->serverHost = _host.data();
	//cl->serverPort = 5900;

	ConnectToRFBServer(cl, _host.c_str(), _port);
	//ConnectToRFBRepeater(cl, _host, _port);
}

void CVncClient::init()
{
	//rfbClientLog = rfbClientErr = log_to_file;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	atexit(SDL_Quit);
	signal(SIGINT, exit);

	/* 16-bit: cl=rfbGetClient(5,3,2); */
	m_cl = rfbGetClient(8, 3, 4);

	//cl->clientData->tag = (void*)12;

	m_cl->MallocFrameBuffer = resize;
	m_cl->canHandleNewFBSize = TRUE;
	m_cl->GotFrameBufferUpdate = update;
	m_cl->HandleKeyboardLedState = kbd_leds;
	m_cl->HandleTextChat = text_chat;
	m_cl->GotXCutText = got_selection;
	m_cl->GetCredential = get_credential;
	m_cl->listenPort = LISTEN_PORT_OFFSET;
	m_cl->listen6Port = LISTEN_PORT_OFFSET;
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
#if defined(_MSC_VER) && _MSC_VER < 1900
				SendPointerEvent(m_cl, _x, _y, BIN8(01000000));
#else
				SendPointerEvent(m_cl, _x, _y, 0b01000000);
#endif
				SendPointerEvent(m_cl, _x, _y, 0);
		}
	if (_wheelX < 0)
		for (steps = 0; steps > _wheelX; --steps)
		{
#if defined(_MSC_VER) && _MSC_VER < 1900
				SendPointerEvent(m_cl, _x, _y, BIN8(00100000));
#else
				SendPointerEvent(m_cl, _x, _y, 0b00100000);
#endif
				SendPointerEvent(m_cl, _x, _y, 0);
		}
}

void CVncClient::OnKey(const SDL_KeyboardEvent& _key, const bool _isDown)
{
    SendKeyEvent(m_cl,
                SDL_key2rfbKeySym(&_key),
                _isDown);
}

rfbBool resize(rfbClient* client)
{
    std::cout << "client->clientData: " << client->clientData << std::endl;

    int width = client->width, height = client->height,
		depth = client->format.bitsPerPixel;
	if (enableResizable)
		sdlFlags |= SDL_WINDOW_RESIZABLE;
	client->updateRect.x = client->updateRect.y = 0;
	client->updateRect.w = width; 
    client->updateRect.h = height;
	/* (re)create the surface used as the client's framebuffer */
	//SDL_FreeSurface(static_cast<SDL_Surface*>(rfbClientGetClientData(client, reinterpret_cast<void*>(&SDL_Init))));
	
    SDL_Surface * sdlSurface = m_window.resize(width, height, depth);

	//rfbClientSetClientData(client, reinterpret_cast<void*>(&SDL_Init), sdl);
	client->width = sdlSurface->pitch / (depth / 8);
	client->frameBuffer = static_cast<uint8_t*>(sdlSurface->pixels);
	client->format.bitsPerPixel = depth;
	client->format.redShift = sdlSurface->format->Rshift;
	client->format.greenShift = sdlSurface->format->Gshift;
	client->format.blueShift = sdlSurface->format->Bshift;
	client->format.redMax = sdlSurface->format->Rmask >> client->format.redShift;
	client->format.greenMax = sdlSurface->format->Gmask >> client->format.greenShift;
	client->format.blueMax = sdlSurface->format->Bmask >> client->format.blueShift;
	SetFormatAndEncodings(client);
        
	return TRUE;
}

void run()
{

	while (1)
	{
		if (SDL_PollEvent(&e))
		{
			/*
			handleSDLEvent() return 0 if user requested window close.
			In this case, handleSDLEvent() will have called cleanup().
			*/
			if (!handleSDLEvent(cl, &e))
				break;
		}
		else
		{
			int i = WaitForMessage(cl, 500);
			if (i<0)
			{
				cleanup(cl);
				return;
			}
			if (i)
				if (!HandleRFBServerMessage(cl))
				{
					cleanup(cl);
					return;
				}
		}
	}
}