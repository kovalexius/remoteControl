#include "sdlScrCapturer.h"

#include <iostream>
#include <string>
#include <stdexcept>

#include <X11/Xlib.h>

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
	std::cout << __FUNCTION__ << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		auto msg = std::string("SDL_Init Error: ") + std::string(SDL_GetError());
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}
	else
	{
		std::cout << "SDL_Init success" << std::endl;
	}

	m_displayMode.driverdata = NULL;
	auto request = SDL_GetDesktopDisplayMode(0, &m_displayMode);
	if(request)
	{
		auto msg = std::string("SDL_GetDesktopDisplayMode: ") + std::string(SDL_GetError());
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}
	else
	{
		std::cout << "SDL_GetDesktopDisplayMode success" << std::endl;
		std::cout << "format: " << m_displayMode.format << std::endl <<
					"w: " << m_displayMode.w << std::endl <<
					"h: " << m_displayMode.h << std::endl <<
					"refresh_rate: " << m_displayMode.refresh_rate << std::endl <<
					"driverdata: " << m_displayMode.driverdata << std::endl;
	}

	//Display* display = XOpenDisplay(NULL);
	//if(display)
	//	m_displayMode.driverdata = (void*)display;

	//m_displayMode.driverdata = (void*)0;
	SDL_Window* m_sdlWindow = SDL_CreateWindowFrom(m_displayMode.driverdata);
	if(m_sdlWindow == NULL)
	{
		auto msg = std::string("failed SDL_CreateWindowFrom: ") + std::string(SDL_GetError());
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}
	else
	{
		std::cout << "SDL_CreateWindowFrom success" << std::endl;
	}

	SDL_Renderer* m_renderer = SDL_CreateRenderer(m_sdlWindow, -1, 0);
	if(m_renderer == NULL)
	{
		auto msg = std::string("failed SDL_CreateRenderer: ") + std::string(SDL_GetError());
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	std::cout << std::string(__FUNCTION__) + " end" << std::endl;
}

bool SDLScrCapturerImpl::getScreenshot(const CRectangle& _region, 
									std::vector<char>& _outBuffer)
{
	std::cout << __FUNCTION__ << std::endl;

	if(m_region != _region)
	{
		try
		{
			m_region = _region;
			initSdl();
		}
		catch(const std::exception& e)
		{
			//std::cout << e.what() << '\n';
		}
		catch(...)
		{
			std::cout << "Undefined exception" << std::endl;
		}
	}

	// Create SDL_Surface with depth of 32 bits
	SDL_Surface *
	_surface = SDL_CreateRGBSurface( 0, _region.getSize().m_x, _region.getSize().m_y, _region.getBitsPerPixel(), 0, 0, 0, 0 );

	// Check if the surface is created properly
	if (_surface == NULL)
	{
		auto msg = std::string("Cannot create SDL_Surface: ") + std::string(SDL_GetError());
		throw std::runtime_error(msg);
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
