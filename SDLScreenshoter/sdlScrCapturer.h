#ifndef __SDL_SCREEN_CAPTURER__H
#define __SDL_SCREEN_CAPTURER__H

#include <vector>

#include "types/geometry_types.h"
#include "SDL2.h"

class SDLScrCapturer
{
public:
    explicit SDLScrCapturer(const CRectangle& _region);

    bool getScreenshot(const CRectangle& _region, 
						std::vector<char>& _outBuffer);
private:
    void initSdl();

    SDL_DisplayMode m_displayMode;
    SDL_Window* m_sdlWindow;

    CRectangle m_region;
}

#endif