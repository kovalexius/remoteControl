#include "SDLvncviewer.h"

#include <iostream>
#include <string>
#include <exception>

 
#include "common_helpers.h"
#include "cpp_legacy_crutchs.h"
#include "log_to_file.h"


static int enableResizable = 1, listenLoop;
int sdlFlags;

/* client's pointer position */
int m_x, m_y;


SDL_Surface* CSdlViewer::resize(const int _width, const int _height, const int _depth)
{
    SDL_FreeSurface(m_sdlSurface);

    m_sdlSurface = SDL_CreateRGBSurface(0,
		_width,
		_height,
		_depth,
		0, 0, 0, 0);
	if (!m_sdlSurface)
		rfbClientErr("resize: error creating surface: %s\n", SDL_GetError());

    /* create or resize the window */
	if (!m_sdlWindow)
	{
		m_sdlWindow = SDL_CreateWindow(client->desktopName,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			sdlFlags);
		if (!m_sdlWindow)
			rfbClientErr("resize: error creating window: %s\n", SDL_GetError());
	}
	else
	{
		SDL_SetWindowSize(m_sdlWindow, _width, _height);
	}

    /* create the renderer if it does not already exist */
	if (!m_sdlRenderer)
	{
		m_sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
		if (!sdlRenderer)
			rfbClientErr("resize: error creating renderer: %s\n", SDL_GetError());
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  /* make the scaled rendering look smoother. */
	}
	SDL_RenderSetLogicalSize(m_sdlRenderer, width, height);  /* this is a departure from the SDL1.2-based version, but more in the sense of a VNC viewer in keeeping aspect ratio */
	/* (re)create the texture that sits in between the surface->pixels and the renderer */
	if (m_sdlTexture)
		SDL_DestroyTexture(sdlTexture);
	m_sdlTexture = SDL_CreateTexture(sdlRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (!m_sdlTexture)
		rfbClientErr("resize: error creating texture: %s\n", SDL_GetError());
}


void CSdlViewer::update(int _x, int _y, int _w, int _h)
{
	/* update texture from surface->pixels */
	SDL_Rect rect = {_x, _y, _w, _h};
    //if (SDL_UpdateTexture(sdlTexture, &rect, sdl->pixels + y * sdl->pitch + x * 4, sdl->pitch) < 0)
	if(SDL_UpdateTexture(m_sdlTexture, &rect, (void*)((char*)m_sdlSurface->pixels + _y * m_sdlSurface->pitch + _x * 4), m_sdlSurface->pitch) < 0)
	    rfbClientErr("update: failed to update texture: %s\n", SDL_GetError());
	/* copy texture to renderer and show */
	if(SDL_RenderClear(m_sdlRenderer) < 0)
	    rfbClientErr("update: failed to clear renderer: %s\n", SDL_GetError());
	if(SDL_RenderCopy(m_sdlRenderer, sdlTexture, NULL, NULL) < 0)
	    rfbClientErr("update: failed to copy texture to renderer: %s\n", SDL_GetError());
	SDL_RenderPresent(m_sdlRenderer);
}

void OnSdlWindowEvent(rfbClient *cl, SDL_Event *_e)
{
	switch (_e->window.event)
	{
		case SDL_WINDOWEVENT_EXPOSED:
			SendFramebufferUpdateRequest(cl, 0, 0, cl->width, cl->height, FALSE);
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

void CSdlViewer::OnSdlMouseWheel(SDL_Event *e)
{
    m_onSdlMouseWheelHandle(m_x, m_y, e->wheel.x, e->wheel.y);
}

void CSdlViewer::OnSdlMouse(SDL_Event *_e)
{
	int state, i;
	if (_e->type == SDL_MOUSEMOTION)
	{
		m_x = _e->motion.x;
		m_y = _e->motion.y;
		state = _e->motion.state;
	}
	else
	{
		m_x = _e->button.x;
		m_y = _e->button.y;
		state = _e->button.button;
		for (i = 0; buttonMapping[i].sdl; i++)
			if (state == buttonMapping[i].sdl)
			{
				state = buttonMapping[i].rfb;
				if (_e->type == SDL_MOUSEBUTTONDOWN)
					m_buttonMask |= state;
				else
					m_buttonMask &= ~state;
				return;
			}
	}
	
    m_onSdlMouseHandle(m_x, m_y, m_buttonMask);

	m_buttonMask &= ~(rfbButton4Mask | rfbButton5Mask);
	return;
}

void CSdlViewer::OnSdlKey(SDL_Event *_e)
{
	m_onSdlKeyHandle(_e->key,
                _e->type == SDL_KEYDOWN ? TRUE : FALSE);
	if (_e->key.keysym.sym == SDLK_RALT)
		m_rightAltKeyDown = _e->type == SDL_KEYDOWN;
	if (_e->key.keysym.sym == SDLK_LALT)
		m_leftAltKeyDown = _e->type == SDL_KEYDOWN;
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

rfbBool handleSDLEvent(rfbClient *cl, SDL_Event *_e)
{
	switch(_e->type)
	{
		case SDL_WINDOWEVENT:
			OnSdlWindowEvent(cl, _e);
			break;
		case SDL_MOUSEWHEEL:
			OnSdlMouseWheel(cl, _e);
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEMOTION:
			OnSdlMouse(cl, _e);
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			OnSdlKey(cl, _e);
			break;
		case SDL_TEXTINPUT:
			OnSdlTextinput(cl, _e);
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
			rfbClientLog("ignore SDL event: 0x%x\n", _e->type);
	}
	return true;
}


#undef main
 

