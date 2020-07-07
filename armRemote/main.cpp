#include <iostream>
#include <ctime>



#include "GDIScreenShooter.h"




//constexpr 
const int WIDTH = 1600;
//constexpr 
const int HEIGHT = 900;
const auto BPP = 4;




int main(int argc,char** argv)
{
	
	

	rfbInitServer(screen);
	rfbRunEventLoop(screen, 4000, FALSE);
	
	return(0);
}