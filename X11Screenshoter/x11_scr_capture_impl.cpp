#include "x11_scr_capture_impl.h"

#include <iostream>

X11ScrCapturerImpl::X11ScrCapturerImpl()
{
	initX11();
}
X11ScrCapturerImpl::~X11ScrCapturerImpl()
{
	if(m_display)
		XCloseDisplay(m_display);
}

void X11ScrCapturerImpl::initX11()
{
	m_display = XOpenDisplay(0);
	if(m_display == NULL)
	{
		std::cout << "failed XOpenDisplay()" << std::endl;
	}
	//int screen = DefaultScreen(display);
	//int display_width = DisplayWidth(display, screen);
	//int display_height = DisplayHeight(display, screen);
	//Window root = XRootWindow(display, screen);
	m_root  = DefaultRootWindow(m_display);

	if(XGetWindowAttributes(m_display, m_root, &m_attributes) == 0)
	{
		std::cout << "failed XGetWindowAttributes()" << std::endl;
	}
}

bool X11ScrCapturerImpl::getScreenshot(const CRectangle& _region, 
						std::vector<char>& _outBuffer)
{
	XImage *image = XGetImage(m_display,
								m_root,
								0, 0,
								m_attributes.width, m_attributes.height,
								AllPlanes,
								ZPixmap);

	/*
	XImage *image = XGetImage(m_display, 
								m_root, 
								_region.getLeftBottom().m_x, 
								_region.getLeftBottom().m_y, 
								_region.getSize().m_x, 
								_region.getSize().m_y, 
								AllPlanes, 
								ZPixmap);
								*/

	_outBuffer.resize(m_attributes.width * m_attributes.height * _region.getBytesPerPixel());

	return true;
}