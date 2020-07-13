#ifndef __SDL_SCREEN_CAPTURER__H
#define __SDL_SCREEN_CAPTURER__H

#include <vector>

#include "SDL.h"
#include "types/geometry_types.h"


class SDLScrCapturerImpl
{
public:
    //explicit SDLScrCapturer(const CRectangle& _region);
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