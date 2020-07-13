#ifndef __X11_SCR_CAPTURE_IMPL__H
#define __X11_SCR_CAPTURE_IMPL__H

#include <vector>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "types/geometry_types.h"

class X11ScrCapturerImpl
{
public:
	X11ScrCapturerImpl();
	~X11ScrCapturerImpl();
	
	bool getScreenshot(const CRectangle& _region, 
						std::vector<char>& _outBuffer);
private:
	void initX11();

	Display* m_display;
	Window m_root;
	XWindowAttributes m_attributes;
};

#endif