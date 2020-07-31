#include "vnc_client.h"
#include "sdl_viewer.h"

#include <string>
#include <string.h>


//#undef main
int main(int argc, char** argv)
{
	bool viewOnly = false;
	bool enableResizable = true;
	bool listenLoop = false;

	std::string host;
	int port = 5901;
    std::string id;
    std::string repeaterHost;
    int repeaterPort = 54320;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-viewonly"))
			viewOnly = true;
		else if (!strcmp(argv[i], "-resizable"))
			enableResizable = true;
		else if (!strcmp(argv[i], "-no-resizable"))
			enableResizable = false;
		else if (!strcmp(argv[i], "-listen"))
			listenLoop = true;
		else if (!strcmp(argv[i], "-host") && i + 1 < argc)
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

	CVncClient client;
    if(!repeaterHost.empty() && !id.empty())
        client.Connect(repeaterHost, repeaterPort, id);
    else if(!repeaterHost.empty() && !host.empty())
        client.Connect(repeaterHost, repeaterPort, host, port);
    else if(!host.empty())
        client.Connect(host, port);
    else
        client.Connect();
	client.Run();
	
	/*
	CSdlViewer view;
	view.resize("AdminKit", 300, 300, 8);

	while (true)
	{
		//if (!view.DoTurn())
		//	break;
		//view.resize(400, 350, 8);
	}
	/**/

	return 0;
}
