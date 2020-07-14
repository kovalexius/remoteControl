#ifndef __X11_SCR_CAPTURE_IMPL__H
#define __X11_SCR_CAPTURE_IMPL__H

#include <vector>

#include <X11/Xlib.h>

#include "types/geometry_types.h"

class X11ScrCapturerImpl
{
public:
	X11ScrCapturerImpl();
	~X11ScrCapturerImpl();
	
	bool getScreenshot(CRectangle& _region, 
						std::vector<char>& _outBuffer);

	CRectangle& getRectangle();
private:
	void initX11();

	Display* m_display;
	Window m_root;

	CRectangle m_region;
};

#endif