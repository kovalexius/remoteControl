#ifndef __SDL_SCREEN_CAPTURER_IMPL__H
#define __SDL_SCREEN_CAPTURER_IMPL__H

#include <vector>

#include "SDL.h"
#include "types/geometry_types.h"


class SDLScrCapturerImpl
{
public:
    //explicit SDLScrCapturerImpl(const CRectangle& _region);
    SDLScrCapturerImpl();

    bool getScreenshot(const CRectangle& _region, 
						std::vector<char>& _outBuffer);
private:
    void initSdl();

    SDL_DisplayMode m_displayMode;
    SDL_Window* m_sdlWindow;
    SDL_Renderer* m_renderer;

    CRectangle m_region;

};

#endif