#include <iostream>
#include <ctime>
#include <memory>

#include "server_holder.h"
#include "screenshot_poly.h"

//constexpr 
const int WIDTH = 1600;
//constexpr 
const int HEIGHT = 900;
const auto BPP = 4;


int main(int argc,char** argv)
{
	auto region = CRectangle(Vector2(0, 0), Vector2(WIDTH, HEIGHT), BPP);
#ifdef _WIN32
	//std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<GDIScrCapture>(region));
	std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<DXScrCapture>());
#else
	//std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<SdlScrCapture>());
	std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<X11ScrCapture>());
#endif
	auto server = ServerHolder::get(region, scrCapture);
	server->run();
	
	return(0);
}