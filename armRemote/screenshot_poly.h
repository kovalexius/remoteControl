#ifndef __SCREENSHOT_POLY__H
#define __SCREENSHOT_POLY__H

#include <vector>
#include "types/geometry_types.h"

#include "GDIScreenShooter.h"
#include "DXScreenShooter.h"
#include "DDrawScreenShooter.h"

class ScrCaptureBase
{
public:
	virtual bool GetScrCapture(const CRectangle& _region, std::vector<char>& _outbuffer) = 0;
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

class DDrawSrcCapture : public ScrCaptureBase
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

#endif

#endif