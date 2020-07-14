#include "x11_scr_capture_impl.h"

#include <iostream>
#include <cstring>

#include <X11/Xutil.h>

X11ScrCapturerImpl::X11ScrCapturerImpl() : m_display(NULL)
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
	else 
	{
		std::cout << "width: " << m_attributes.width <<
					"height: " << m_attributes.height <<std::endl;
	}
}

bool X11ScrCapturerImpl::getScreenshot(const CRectangle& _region, 
						std::vector<char>& _outBuffer)
{
	//std::cout << __FUNCTION__ << std::endl;

	/*
	XImage *image = XGetImage(m_display,
								m_root,
								0, 0,
								m_attributes.width, m_attributes.height,
								AllPlanes,
								ZPixmap);
	*/

	//*
	XImage *image = XGetImage(m_display, 
								m_root, 
								_region.getLeftBottom().m_x, 
								_region.getLeftBottom().m_y, 
								_region.getSize().m_x, 
								_region.getSize().m_y, 
								AllPlanes, 
								ZPixmap);
	/**/

	if(image == NULL)
	{
		std::cout << "XGetImage failed " << std::endl;
		return false;
	}

	//std::cout << "XGetImage success" <<std::endl;

	//_outBuffer.resize(m_attributes.width * m_attributes.height * _region.getBytesPerPixel());
	_outBuffer.resize(_region.getSize().m_x * _region.getSize().m_y * _region.getBytesPerPixel());

	std::memcpy(_outBuffer.data(), image->data, _outBuffer.size());

	XDestroyImage(image);

	return true;
}