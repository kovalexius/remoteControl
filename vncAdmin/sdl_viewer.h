#ifndef __SDL_VIEWER__H
#define __SDL_VIEWER__H


#include <functional>
#include <rfb/rfbclient.h>
#include <SDL.h>



class CSdlViewer
{
public:
	CSdlViewer();
	~CSdlViewer();

    SDL_Surface* resize(const int _width, const int _height, const int _depth);
    void update(int x,int y,int w,int h);

    virtual void OnSdlMouse(SDL_Event *_e);
    virtual void OnSdlMouseWheel(SDL_Event *_e);
    virtual void OnSdlKey(SDL_Event *_e);

private:
    SDL_Surface *m_sdlSurface;
    SDL_Window *m_sdlWindow;
    SDL_Renderer *m_sdlRenderer;
    SDL_Texture *m_sdlTexture;

    std::function<void(int m_x, int m_y, int m_buttonMask)> m_onSdlMouseHandle;
    std::function<void(const int _x, const int _y, const int _wheelX, const int _wheelY)>  m_onSdlMouseWheelHandle;
    std::function<void(const SDL_KeyboardEvent& _key, const bool _isDown)> m_onSdlKeyHandle;

    int m_rightAltKeyDown;
    int m_leftAltKeyDown;
    int m_buttonMask;
};

#endif