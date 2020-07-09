#include "sdlScrCapturer.h"

SDLScrCapturer::SDLScrCapturer(const CRectangle& _region) :
                                                            m_region(_region)
{
}

void SDLScrCapturer::initSdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
	    throw std::string("SDL_Init Error: ") + std::string(SDL_GetError());
    }

    
}

bool SDLScrCapturer::getScreenshot(const CRectangle& _region, 
						std::vector<char>& _outBuffer)
{
      // Used temporary variables
    SDL_Rect _viewport;
    SDL_Surface *_surface = NULL;

    // Get viewport size
    SDL_RenderGetViewport( renderer, &_viewport);

    // Create SDL_Surface with depth of 32 bits
    _surface = SDL_CreateRGBSurface( 0, _viewport.w, _viewport.h, 32, 0, 0, 0, 0 );

    // Check if the surface is created properly
    if ( _surface == NULL ) {
      std::cout << "Cannot create SDL_Surface: " << SDL_GetError() << std::endl;
      return false;
     }

    // Get data from SDL_Renderer and save them into surface
    if ( SDL_RenderReadPixels( renderer, NULL, _surface->format->format, _surface->pixels, _surface->pitch ) != 0 ) {
      std::cout << "Cannot read data from SDL_Renderer: " << SDL_GetError() << std::endl;
    
      // Don't forget to free memory
      SDL_FreeSurface(_surface);
      return false;
    }

    // Save screenshot as PNG file
    if ( IMG_SavePNG( _surface, file.c_str() ) != 0 ) {
      std::cout << "Cannot save PNG file: " << SDL_GetError() << std::endl;
    
      // Free memory
      SDL_FreeSurface(_surface);
      return false;
    }

    // Free memory
    SDL_FreeSurface(_surface);
    return true;    

}