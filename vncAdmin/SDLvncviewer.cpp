#include "SDLvncviewer.h"

#include <iostream>
#include <string>


 
#include "common_helpers.h"
#include "cpp_legacy_crutchs.h"
#include "log_to_file.h"



static int enableResizable = 1, viewOnly, listenLoop, buttonMask;
int sdlFlags;


SDL_Texture *sdlTexture;
SDL_Renderer *sdlRenderer;
SDL_Window *sdlWindow;
/* client's pointer position */
int m_x, m_y;
 
static int rightAltKeyDown, leftAltKeyDown;


 
rfbBool resize(rfbClient* client)
{
	std::cout << "client->clientData: " << client->clientData << std::endl;

	int width = client->width, height = client->height,
		depth = client->format.bitsPerPixel;
	if (enableResizable)
		sdlFlags |= SDL_WINDOW_RESIZABLE;
	client->updateRect.x = client->updateRect.y = 0;
	client->updateRect.w = width; client->updateRect.h = height;
	/* (re)create the surface used as the client's framebuffer */
	SDL_FreeSurface(static_cast<SDL_Surface*>(rfbClientGetClientData(client, reinterpret_cast<void*>(&SDL_Init))));
	SDL_Surface* sdl = SDL_CreateRGBSurface(0,
		width,
		height,
		depth,
		0, 0, 0, 0);
	if (!sdl)
		rfbClientErr("resize: error creating surface: %s\n", SDL_GetError());

	rfbClientSetClientData(client, reinterpret_cast<void*>(&SDL_Init), sdl);
	client->width = sdl->pitch / (depth / 8);
	client->frameBuffer = static_cast<uint8_t*>(sdl->pixels);
	client->format.bitsPerPixel = depth;
	client->format.redShift = sdl->format->Rshift;
	client->format.greenShift = sdl->format->Gshift;
	client->format.blueShift = sdl->format->Bshift;
	client->format.redMax = sdl->format->Rmask >> client->format.redShift;
	client->format.greenMax = sdl->format->Gmask >> client->format.greenShift;
	client->format.blueMax = sdl->format->Bmask >> client->format.blueShift;
	SetFormatAndEncodings(client);
	/* create or resize the window */
	if (!sdlWindow)
	{
		sdlWindow = SDL_CreateWindow(client->desktopName,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			sdlFlags);
		if (!sdlWindow)
			rfbClientErr("resize: error creating window: %s\n", SDL_GetError());
	}
	else
	{
		SDL_SetWindowSize(sdlWindow, width, height);
	}
	/* create the renderer if it does not already exist */
	if (!sdlRenderer)
	{
		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
		if (!sdlRenderer)
			rfbClientErr("resize: error creating renderer: %s\n", SDL_GetError());
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  /* make the scaled rendering look smoother. */
	}
	SDL_RenderSetLogicalSize(sdlRenderer, width, height);  /* this is a departure from the SDL1.2-based version, but more in the sense of a VNC viewer in keeeping aspect ratio */
	/* (re)create the texture that sits in between the surface->pixels and the renderer */
	if (sdlTexture)
		SDL_DestroyTexture(sdlTexture);
	sdlTexture = SDL_CreateTexture(sdlRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (!sdlTexture)
		rfbClientErr("resize: error creating texture: %s\n", SDL_GetError());
	return TRUE;
}

 
void update(rfbClient* cl,int x,int y,int w,int h) 
{
	SDL_Surface *sdl = static_cast<SDL_Surface*>(rfbClientGetClientData(cl, reinterpret_cast<void*>(&SDL_Init)));
	/* update texture from surface->pixels */
	SDL_Rect r = {x,y,w,h};
	if(SDL_UpdateTexture(sdlTexture, &r, (void*)((char*)sdl->pixels + y*sdl->pitch + x*4), sdl->pitch) < 0)		 //if (SDL_UpdateTexture(sdlTexture, &r, sdl->pixels + y * sdl->pitch + x * 4, sdl->pitch) < 0)
	    rfbClientErr("update: failed to update texture: %s\n", SDL_GetError());
	/* copy texture to renderer and show */
	if(SDL_RenderClear(sdlRenderer) < 0)
	    rfbClientErr("update: failed to clear renderer: %s\n", SDL_GetError());
	if(SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL) < 0)
	    rfbClientErr("update: failed to copy texture to renderer: %s\n", SDL_GetError());
	SDL_RenderPresent(sdlRenderer);
}

void OnSdlWindowEvent(rfbClient *cl, SDL_Event *e)
{
	switch (e->window.event)
	{
		case SDL_WINDOWEVENT_EXPOSED:
			SendFramebufferUpdateRequest(cl, 0, 0,
				cl->width, cl->height, FALSE);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			if (SDL_HasClipboardText())
			{
				char *text = SDL_GetClipboardText();
				if (text)
				{
					rfbClientLog("sending clipboard text '%s'\n", text);
					SendClientCutText(cl, text, strlen(text));
				}
			}
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			if (rightAltKeyDown)
			{
				SendKeyEvent(cl, XK_Alt_R, FALSE);
				rightAltKeyDown = FALSE;
				rfbClientLog("released right Alt key\n");
			}
			if (leftAltKeyDown)
			{
				SendKeyEvent(cl, XK_Alt_L, FALSE);
				leftAltKeyDown = FALSE;
				rfbClientLog("released left Alt key\n");
			}
			break;
	}
}

void OnSdlMouseWheel(rfbClient *cl, SDL_Event *e)
{
	int steps;
	if (viewOnly)
		return;

	if (e->wheel.y > 0)
		for (steps = 0; steps < e->wheel.y; ++steps)
		{
			SendPointerEvent(cl, m_x, m_y, rfbButton4Mask);
			SendPointerEvent(cl, m_x, m_y, 0);
		}
	if (e->wheel.y < 0)
		for (steps = 0; steps > e->wheel.y; --steps)
		{
			SendPointerEvent(cl, m_x, m_y, rfbButton5Mask);
			SendPointerEvent(cl, m_x, m_y, 0);
		}
	if (e->wheel.x > 0)
		for (steps = 0; steps < e->wheel.x; ++steps)
		{
#if defined(_MSC_VER) && _MSC_VER < 1900
				SendPointerEvent(cl, m_x, m_y, BIN8(01000000));
#else
				SendPointerEvent(cl, m_x, m_y, 0b01000000);
#endif
				SendPointerEvent(cl, m_x, m_y, 0);
		}
	if (e->wheel.x < 0)
		for (steps = 0; steps > e->wheel.x; --steps)
		{
#if defined(_MSC_VER) && _MSC_VER < 1900
				SendPointerEvent(cl, m_x, m_y, BIN8(00100000));
#else
				SendPointerEvent(cl, m_x, m_y, 0b00100000);
#endif
				SendPointerEvent(cl, m_x, m_y, 0);
		}
	return;
}

void OnSdlMouse(rfbClient *cl, SDL_Event *e)
{
	int state, i;
	if (viewOnly)
		return;
	if (e->type == SDL_MOUSEMOTION)
	{
		m_x = e->motion.x;
		m_y = e->motion.y;
		state = e->motion.state;
	}
	else
	{
		m_x = e->button.x;
		m_y = e->button.y;
		state = e->button.button;
		for (i = 0; buttonMapping[i].sdl; i++)
			if (state == buttonMapping[i].sdl)
			{
				state = buttonMapping[i].rfb;
				if (e->type == SDL_MOUSEBUTTONDOWN)
					buttonMask |= state;
				else
					buttonMask &= ~state;
				return;
			}
	}
	SendPointerEvent(cl, m_x, m_y, buttonMask);
	buttonMask &= ~(rfbButton4Mask | rfbButton5Mask);
	return;
}

void OnSdlKey(rfbClient *cl, SDL_Event *e)
{
	if (viewOnly)
		return;
	SendKeyEvent(cl, SDL_key2rfbKeySym(&e->key), e->type == SDL_KEYDOWN ? TRUE : FALSE);
	if (e->key.keysym.sym == SDLK_RALT)
		rightAltKeyDown = e->type == SDL_KEYDOWN;
	if (e->key.keysym.sym == SDLK_LALT)
		leftAltKeyDown = e->type == SDL_KEYDOWN;
}

void OnSdlTextinput(rfbClient *cl, SDL_Event *e)
{
	if (viewOnly)
		return;
	rfbKeySym sym = utf8char2rfbKeySym(e->text.text);
	SendKeyEvent(cl, sym, TRUE);
	SendKeyEvent(cl, sym, FALSE);
	return;
}

rfbBool handleSDLEvent(rfbClient *cl, SDL_Event *e)
{
	switch(e->type)
	{
		case SDL_WINDOWEVENT:
			OnSdlWindowEvent(cl, e);
			break;
		case SDL_MOUSEWHEEL:
			OnSdlMouseWheel(cl, e);
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEMOTION:
			OnSdlMouse(cl, e);
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			OnSdlKey(cl, e);
			break;
		case SDL_TEXTINPUT:
			OnSdlTextinput(cl, e);
			break;
        case SDL_QUIT:
			if(listenLoop)
			{
				cleanup(cl);
				return false;
			}
			else
			{
				rfbClientCleanup(cl);
				return true;
			}
        default:
			rfbClientLog("ignore SDL event: 0x%x\n", e->type);
	}
	return true;
}


#undef main
 

