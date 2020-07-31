#include <iostream>
#include <ctime>
#include <memory>

#include "server_holder.h"
#include "screenshot_poly.h"


int main(int argc,char** argv)
{
    std::string host;
	int port = 5501;
    std::string id;
    std::string repeaterHost;
    int repeaterPort = 54321;

    for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-host") && i + 1 < argc)
			host = argv[i+1];
		else if (!strcmp(argv[i], "-port") && i + 1 < argc)
			port = std::stoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-id") && i + 1 < argc)
            id = argv[i+1];
        else if (!strcmp(argv[i], "-repeaterHost") && i + 1 < argc)
            repeaterHost = argv[i+1];
        else if (!strcmp(argv[i], "-repeaterPort") && i + 1 < argc)
            repeaterPort = std::stoi(argv[i + 1]);
	}

#ifdef _WIN32
	//std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<GDIScrCapture>(region));
	std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<DXScrCapture>());
#else
	//std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<SdlScrCapture>());
	std::shared_ptr<ScrCaptureBase> scrCapture(std::make_shared<X11ScrCapture>());
#endif
    auto server = ServerHolder::get(scrCapture);

    if(!repeaterHost.empty() && !id.empty())
        server->Connect(repeaterHost, repeaterPort, id);
    else if(!host.empty())
        server->Connect(host, port);
    else
        server->Listen();
    
	server->Run();
	
	return(0);
}