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

	SDL_Surface* getSurface();
    SDL_Surface* resize(const std::string& _desktopName, const int _width, const int _height, const int _depth);
    void update(int x,int y,int w,int h);
	bool  DoTurn();

	void SubscribeOnSdlMouse(std::function<void(int m_x, int m_y, int m_buttonMask)> _onSdlMouseHandle);
	void SubscribeOnSdlMouseWheel(std::function<void(const int _x, const int _y, const int _wheelX, const int _wheelY)> _onSdlMouseWheelHandle);
	void SubscribeOnSdlKey(std::function<void(const SDL_KeyboardEvent& _key, const bool _isDown)> _onSdlKeyHandle);
	void SubscribeOnSdlTextinput(std::function<void(const std::string& _chr)> _onSdlTextinput);
	void SubscribeOnSdlWindowExposed(std::function<void()> _onSdlWindowExposed);
	void SubscribeOnSdlKeyboardFocus(std::function<void(const std::string& _text)> _onSdlKeyboardFocus);
	void SubscribemOnSdlQuit(std::function<void()> _onSdlQuit);
	void SubscribemOnSdlWindowFocusLost(std::function<void(const bool _rightAltDown, const bool _leftAltDown)>  _onSdlWindowFocusLost);

protected:
	virtual void OnSdlMouse(SDL_Event *_e);
	virtual void OnSdlMouseWheel(SDL_Event *_e);
	virtual void OnSdlKey(SDL_Event *_e);
	virtual void OnSdlTextinput(SDL_Event *_e);
	virtual void OnSdlWindowEvent(SDL_Event *_e);
	virtual void OnSdlWindowExposed(SDL_Event *_e);
	virtual void OnSdlKeyboardFocus(SDL_Event *_e);
	virtual void OnSdlWindowFocusLost(SDL_Event *_e);
	virtual void OnSdlQuit(SDL_Event *_e);
private:
	bool handleSDLEvent(SDL_Event *_e);

	std::function<void(int m_x, int m_y, int m_buttonMask)> m_onSdlMouseHandle;
	std::function<void(const int _x, const int _y, const int _wheelX, const int _wheelY)>  m_onSdlMouseWheelHandle;
	std::function<void(const SDL_KeyboardEvent& _key, const bool _isDown)> m_onSdlKeyHandle;
	std::function<void(const std::string& _chr)> m_onSdlTextinput;
	std::function<void()> m_onSdlWindowExposed;
	std::function<void(const std::string& _text)> m_onSdlKeyboardFocus;
	std::function<void()> m_onSdlQuit;
	std::function<void(const bool _rightAltDown, const bool _leftAltDown)>  m_onSdlWindowFocusLost;

	std::string m_desktopName;

    SDL_Surface *m_sdlSurface;
    SDL_Window *m_sdlWindow;
    SDL_Renderer *m_sdlRenderer;
    SDL_Texture *m_sdlTexture;
	SDL_Event m_event;

	Uint32 m_sdlFlags;

    bool m_rightAltKeyDown;
    bool m_leftAltKeyDown;
    int m_buttonMask;

	/* client's pointer position */
	int m_x;
	int m_y;
};

#endif