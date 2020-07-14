#ifndef __SCREENSHOT_POLY__H
#define __SCREENSHOT_POLY__H

#include <vector>
#include "types/geometry_types.h"

#ifdef _WIN32
#include "GDIScreenShooter.h"
#include "DXScreenShooter.h"
#include "DDrawScreenShooter.h"
#elif __linux__
#include "sdlScrCapturer.h"
#include "x11_scr_capture_impl.h"
#endif

class ScrCaptureBase
{
public:
	virtual bool GetScrCapture(CRectangle& _region, std::vector<char>& _outbuffer) = 0;
	virtual CRectangle& getRectangle() = 0;
};


#ifdef _WIN32
class GDIScrCapture : public ScrCaptureBase
{
public:
	GDIScrCapture(const CRectangle& _region) : m_shooter(_region)
	{}

	bool GetScrCapture(const CRectangle& _region, std::vector<char>& _outbuffer) override
	{
		if (!m_shooter.getScreenshot(_region, _outbuffer))
			return false;
		return true;
	}
private:
	CGDIScreenShooter m_shooter;
};

class DXScrCapture : public ScrCaptureBase
{
public:
	bool GetScrCapture(const CRectangle& _region, std::vector<char>& _outbuffer) override
	{
		if (!m_shooter.GetScreenShot(_region, _outbuffer))
			return false;
		return true;
	}
private:
	CDxScreenShooter m_shooter;
};

class DDrawScrCapture : public ScrCaptureBase
{
public:
	bool GetScrCapture(const CRectangle& _region, std::vector<char>& _outbuffer) override
	{
		if (!m_shooter.GetScreenShot(_region, _outbuffer))
			return false;
		return true;
	}
private:
	CDDrawScreenShooter m_shooter;
};
#elif __linux__
class SdlScrCapture : public ScrCaptureBase
{
public:
	bool GetScrCapture(CRectangle& _region, std::vector<char>& _outbuffer) override
	{
		if(!m_shooter.getScreenshot(_region, _outbuffer))
			return false;
		return true;
	}

	CRectangle& getRectangle() override
	{
		return m_shooter.getRectangle();
	}

private:
	SDLScrCapturerImpl m_shooter;
};

class X11ScrCapture : public ScrCaptureBase
{
public:
	bool GetScrCapture(CRectangle& _region, std::vector<char>& _outbuffer) override
	{
		if(!m_shooter.getScreenshot(_region, _outbuffer))
			return false;
		return true;
	}

	CRectangle& getRectangle() override
	{
		return m_shooter.getRectangle();
	}

private:
	X11ScrCapturerImpl m_shooter;
};

#endif

#endif