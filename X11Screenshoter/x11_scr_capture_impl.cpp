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
		throw std::runtime_error("failed XOpenDisplay()");
	}

	m_root  = DefaultRootWindow(m_display);

	XWindowAttributes attributes;
	if(XGetWindowAttributes(m_display, m_root, &attributes) == 0)
	{
		throw std::runtime_error("failed XGetWindowAttributes()");
	}
	else 
	{
		m_region.getLeftBottom().m_x = attributes.x;
		m_region.getLeftBottom().m_y = attributes.y;
		m_region.getSize().m_x = attributes.width;
		m_region.getSize().m_y = attributes.height;
		m_region.setBitsPerPixel(attributes.depth);
	}
}

CRectangle& X11ScrCapturerImpl::getRectangle()
{
	return m_region;
}

bool X11ScrCapturerImpl::getScreenshot(CRectangle& _region, 
						std::vector<char>& _outBuffer)
{
	//std::cout << "m_region: " << m_region << std::endl;
	XImage *image = XGetImage(m_display, 
								m_root, 
								m_region.getLeftBottom().m_x, 
								m_region.getLeftBottom().m_y, 
								m_region.getSize().m_x, 
								m_region.getSize().m_y, 
								AllPlanes, 
								ZPixmap);

	if(image == NULL)
	{
		std::cout << "XGetImage failed " << std::endl;
		return false;
	}

	_region.getSize().m_x = image->width;
	_region.getSize().m_y = image->height;
	_region.setBitsPerPixel(image->bitmap_pad);
	
	/*
	std::cout << "image.width: " << image->width <<  std::endl <<
				"image.height: " << image->height << std::endl <<
				"image.format: " << image->format << std::endl <<
				"image.byte_order: " << image->byte_order << std::endl <<
				"image.bitmap_pad: " << image->bitmap_pad << std::endl <<
				"image.depth: " << image->depth << std::endl <<
				"image.bits_per_pixel: " << image->bits_per_pixel << std::endl;
	/**/

	_outBuffer.resize(_region.getSize().m_x * _region.getSize().m_y * _region.getBytesPerPixel());
	m_region = _region;
	//std::cout << "m_region: " << m_region << std::endl;
	std::memcpy(_outBuffer.data(), image->data, _outBuffer.size());

	XDestroyImage(image);

	return true;
}