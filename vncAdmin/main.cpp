#include "SDLvncviewer.h"

#include <string>
#include <string.h>

#undef main

int main(int argc, char** argv)
{
	bool viewOnly = false;
	bool enableResizable = true;
	bool listenLoop = false;

	std::string host;
	int port = 5901;

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
		{
			host = argv[i + 1];
		}
		else if (!strcmp(argv[i], "-port") && i + 1 < argc)
		{
			port = std::stoi(argv[i + 1]);
		}
	}

	run();
	

	return 0;
}
