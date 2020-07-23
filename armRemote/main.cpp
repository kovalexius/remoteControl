#include <iostream>
#include <ctime>
#include <memory>

#include "server_holder.h"
#include "screenshot_poly.h"


int main(int argc,char** argv)
{
	//auto region = CRectangle(Vector2(0, 0), Vector2(WIDTH, HEIGHT), BPP);
#ifdef _WIN32
	//std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<GDIScrCapture>(region));
	std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<DXScrCapture>());
#else
	//std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<SdlScrCapture>());
	std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<X11ScrCapture>());
#endif
	//auto server = ServerHolder::get(scrCapture);
	//auto server = ServerHolder::get(scrCapture, std::string("172.18.4.58"), 5501);
	auto server = ServerHolder::get(scrCapture, std::string("172.18.4.58"), 5501, "arm1");
	//auto server = ServerHolder::get(scrCapture, std::string("localhost"), 5500, "arm1");
	server->run();
	
	return(0);
}