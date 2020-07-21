#include "sdl_viewer.h"

#include <iostream>
#include <string>
#include <exception>
 
#include "common_helpers.h"
#include "log_to_file.h"


CSdlViewer::CSdlViewer() : m_sdlSurface(nullptr),
							m_sdlWindow(nullptr),
							m_sdlRenderer(nullptr),
							m_sdlTexture(nullptr),
							m_desktopName("window")
{
	m_sdlFlags = SDL_WINDOW_RESIZABLE;
}

CSdlViewer::~CSdlViewer()
{
	if (m_sdlTexture)
		SDL_DestroyTexture(m_sdlTexture);
	if(m_sdlRenderer)
		SDL_DestroyRenderer(m_sdlRenderer);
	if(m_sdlSurface)
		SDL_FreeSurface(m_sdlSurface);
	if(m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);
	
	SDL_Quit();
}

bool CSdlViewer::DoTurn()
{
	if (SDL_PollEvent(&m_event))
	{
		/*
		handleSDLEvent() return 0 if user requested window close.
		In this case, handleSDLEvent() will have called cleanup().
		*/
		if (!handleSDLEvent(&m_event))
			return false;
	}
	return true;
}

SDL_Surface* CSdlViewer::getSurface()
{
	return m_sdlSurface;
}

SDL_Surface* CSdlViewer::resize(const std::string& _desktopName, const int _width, const int _height, const int _depth)
{
	if (m_sdlSurface)
	{
		SDL_FreeSurface(m_sdlSurface);
		m_sdlSurface = nullptr;
	}

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
		m_desktopName = _desktopName;
		m_sdlWindow = SDL_CreateWindow(m_desktopName.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			_width,
			_height,
			m_sdlFlags);
		if (!m_sdlWindow)
			rfbClientErr("resize: error creating window: %s\n", SDL_GetError());
	}
	
	SDL_SetWindowSize(m_sdlWindow, _width, _height);
	
    /* create the renderer if it does not already exist */
	if (!m_sdlRenderer)
	{
		m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, 0);
		if (!m_sdlRenderer)
			rfbClientErr("resize: error creating renderer: %s\n", SDL_GetError());
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  /* make the scaled rendering look smoother. */
	}
	SDL_RenderSetLogicalSize(m_sdlRenderer, _width, _height);  /* this is a departure from the SDL1.2-based version, but more in the sense of a VNC viewer in keeeping aspect ratio */
	/* (re)create the texture that sits in between the surface->pixels and the renderer */
	if (m_sdlTexture)
	{
		SDL_DestroyTexture(m_sdlTexture);
		m_sdlTexture = nullptr;
	}
	m_sdlTexture = SDL_CreateTexture(m_sdlRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		_width, _height);
	if (!m_sdlTexture)
		rfbClientErr("resize: error creating texture: %s\n", SDL_GetError());

	return m_sdlSurface;
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
	if(SDL_RenderCopy(m_sdlRenderer, m_sdlTexture, NULL, NULL) < 0)
	    rfbClientErr("update: failed to copy texture to renderer: %s\n", SDL_GetError());
	SDL_RenderPresent(m_sdlRenderer);
}

void CSdlViewer::SubscribeOnSdlMouse(std::function<void(int m_x, int m_y, int m_buttonMask)> _onSdlMouseHandle)
{
	m_onSdlMouseHandle = _onSdlMouseHandle;
}

void CSdlViewer::SubscribeOnSdlMouseWheel(std::function<void(const int _x, const int _y, const int _wheelX, const int _wheelY)> _onSdlMouseWheelHandle)
{
	m_onSdlMouseWheelHandle = _onSdlMouseWheelHandle;
}

void CSdlViewer::SubscribeOnSdlKey(std::function<void(const SDL_KeyboardEvent& _key, const bool _isDown)> _onSdlKeyHandle)
{
	m_onSdlKeyHandle = _onSdlKeyHandle;
}

void CSdlViewer::SubscribeOnSdlTextinput(std::function<void(const std::string& _chr)> _onSdlTextinput)
{
	m_onSdlTextinput = _onSdlTextinput;
}

void CSdlViewer::SubscribeOnSdlWindowExposed(std::function<void()> _onSdlWindowExposed)
{
	m_onSdlWindowExposed = _onSdlWindowExposed;
}

void CSdlViewer::SubscribeOnSdlKeyboardFocus(std::function<void(const std::string& _text)> _onSdlKeyboardFocus)
{
	m_onSdlKeyboardFocus = _onSdlKeyboardFocus;
}

void CSdlViewer::SubscribemOnSdlQuit(std::function<void()> _onSdlQuit)
{
	m_onSdlQuit = _onSdlQuit;
}

void CSdlViewer::SubscribemOnSdlWindowFocusLost(std::function<void(const bool _rightAltDown, const bool _leftAltDown)>  _onSdlWindowFocusLost)
{
	m_onSdlWindowFocusLost = _onSdlWindowFocusLost;
}

void CSdlViewer::OnSdlWindowExposed(SDL_Event *_e)
{
	if (m_onSdlWindowExposed)
		m_onSdlWindowExposed();
}

void CSdlViewer::OnSdlKeyboardFocus(SDL_Event *_e)
{
	if (SDL_HasClipboardText())
	{
		char *text = SDL_GetClipboardText();
		if (text)
			if(m_onSdlKeyboardFocus) 
				m_onSdlKeyboardFocus(text);
	}
}

void CSdlViewer::OnSdlWindowFocusLost(SDL_Event *_e)
{
	if (m_onSdlWindowFocusLost)
		m_onSdlWindowFocusLost(m_rightAltKeyDown, m_leftAltKeyDown);
	m_rightAltKeyDown = false;
	m_leftAltKeyDown = false;
}

void CSdlViewer::OnSdlQuit(SDL_Event *_e)
{
	if (m_onSdlQuit)
		m_onSdlQuit();
}

void CSdlViewer::OnSdlWindowEvent(SDL_Event *_e)
{
	switch (_e->window.event)
	{
		case SDL_WINDOWEVENT_EXPOSED:
			OnSdlWindowExposed(_e);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			OnSdlKeyboardFocus(_e);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			OnSdlWindowFocusLost(_e);
			break;
	}
}

void CSdlViewer::OnSdlMouseWheel(SDL_Event *e)
{
	if (m_onSdlMouseWheelHandle)
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
	
	if (m_onSdlMouseHandle)
		m_onSdlMouseHandle(m_x, m_y, m_buttonMask);

	m_buttonMask &= ~(rfbButton4Mask | rfbButton5Mask);
	return;
}

void CSdlViewer::OnSdlKey(SDL_Event *_e)
{
	if (m_onSdlKeyHandle)
		m_onSdlKeyHandle(_e->key,
                _e->type == SDL_KEYDOWN ? TRUE : FALSE);
	if (_e->key.keysym.sym == SDLK_RALT)
		m_rightAltKeyDown = _e->type == SDL_KEYDOWN;
	if (_e->key.keysym.sym == SDLK_LALT)
		m_leftAltKeyDown = _e->type == SDL_KEYDOWN;
}

void CSdlViewer::OnSdlTextinput(SDL_Event *e)
{
	if (m_onSdlTextinput)
		m_onSdlTextinput(e->text.text);
	return;
}

bool CSdlViewer::handleSDLEvent(SDL_Event *_e)
{
	switch(_e->type)
	{
		case SDL_WINDOWEVENT:
			OnSdlWindowEvent(_e);
			break;
		case SDL_MOUSEWHEEL:
			OnSdlMouseWheel(_e);
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEMOTION:
			OnSdlMouse(_e);
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			OnSdlKey(_e);
			break;
		case SDL_TEXTINPUT:
			OnSdlTextinput(_e);
			break;
        case SDL_QUIT:
			OnSdlQuit(_e);
			return false;
        default:
			rfbClientLog("ignore SDL event: 0x%x\n", _e->type);
	}
	return true;
}

 

