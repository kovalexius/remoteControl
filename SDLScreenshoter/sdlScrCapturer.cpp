#include "sdlScrCapturer.h"

#include <iostream>
#include <string>

//#include <X11/Xlib.h>

//SDLScrCapturer::SDLScrCapturer(const CRectangle& _region) :
//                                                            m_region(_region)
//{
//initSdl();
//}

SDLScrCapturerImpl::SDLScrCapturerImpl()
{
	//initSdl();
}

void SDLScrCapturerImpl::initSdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		throw std::string("SDL_Init Error: ") + std::string(SDL_GetError());

	auto request = SDL_GetDesktopDisplayMode(0, &m_displayMode);
	if(!request)
		throw std::string("SDL_GetDesktopDisplayMode: ") + std::string(SDL_GetError());


	//Display* display = XOpenDisplay(NULL);
	SDL_Window* m_sdlWindow = SDL_CreateWindowFrom(m_displayMode.driverdata);
	if(m_sdlWindow == NULL)
		throw std::string("failed SDL_CreateWindowFrom: ") + std::string(SDL_GetError());

	SDL_Renderer* m_renderer = SDL_CreateRenderer(m_sdlWindow, -1, 0);
	if(m_renderer == NULL)
		throw std::string("failed SDL_CreateRenderer: ") + std::string(SDL_GetError());
}

bool SDLScrCapturerImpl::getScreenshot(const CRectangle& _region, 
									std::vector<char>& _outBuffer)
{
	if(m_region != _region)
	{
		m_region = _region;
		initSdl();
	}

	// Create SDL_Surface with depth of 32 bits
	SDL_Surface *
	_surface = SDL_CreateRGBSurface( 0, _region.getSize().m_x, _region.getSize().m_y, _region.getBitsPerPixel(), 0, 0, 0, 0 );

	// Check if the surface is created properly
	if (_surface == NULL)
	{
		throw std::string("Cannot create SDL_Surface: ") + std::string(SDL_GetError());
	}

	// Get data from SDL_Renderer and save them into surface
	if (SDL_RenderReadPixels(m_renderer, NULL, _surface->format->format, _surface->pixels, _surface->pitch ) != 0)
	{
		std::cout << "Cannot read data from SDL_Renderer: " << SDL_GetError() << std::endl;
    
		// Don't forget to free memory
		SDL_FreeSurface(_surface);
		return false;
	}

    // Free memory
    SDL_FreeSurface(_surface);
    return true;    
}
