#ifndef __SDL_CLIENT__H
#define __SDL_CLIENT__H

#include <string>

class CSdlClient
{
public:
	CSdlClient();
	CSdlClient(const std::string& _host, int _port);

	void run();
private:
	void init();

	rfbClient* cl;
	SDL_Event e;
};

#endif