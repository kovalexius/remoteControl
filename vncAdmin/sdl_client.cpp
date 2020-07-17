#include "sdl_client.h"
#include "SDLvncviewer.h"

#include <signal.h>

#include "common_helpers.h"

CSdlClient::CSdlClient()
{
	init();

	if (!rfbInitClient(cl, nullptr, nullptr))
	{
		cleanup(cl);
		cl = NULL; /* rfbInitClient has already freed the client struct */

		return;
	}
}

CSdlClient::CSdlClient(const std::string& _host, int _port)
{
	init();

	//cl->serverHost = _host.data();
	//cl->serverPort = 5900;

	ConnectToRFBServer(cl, _host.c_str(), _port);
	//ConnectToRFBRepeater(cl, _host, _port);
}

void CSdlClient::init()
{
	//rfbClientLog = rfbClientErr = log_to_file;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	atexit(SDL_Quit);
	signal(SIGINT, exit);

	/* 16-bit: cl=rfbGetClient(5,3,2); */
	cl = rfbGetClient(8, 3, 4);

	//cl->clientData->tag = (void*)12;

	cl->MallocFrameBuffer = resize;
	cl->canHandleNewFBSize = TRUE;
	cl->GotFrameBufferUpdate = update;
	cl->HandleKeyboardLedState = kbd_leds;
	cl->HandleTextChat = text_chat;
	cl->GotXCutText = got_selection;
	cl->GetCredential = get_credential;
	cl->listenPort = LISTEN_PORT_OFFSET;
	cl->listen6Port = LISTEN_PORT_OFFSET;
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