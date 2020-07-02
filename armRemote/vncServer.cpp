//#define snprintf _snprintf
/*
#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#else
#include <stdio.h> //sprintf
#endif
*/

#include <iostream>
#include <ctime>

#include <rfb/rfb.h>




const int WIDTH = 1600;
const int HEIGHT = 900;
const auto BPP = 4;


static void initBuffer(unsigned char* buffer)
{
	int i, j;
	for (j = 0; j<HEIGHT; ++j) {
		for (i = 0; i<WIDTH; ++i) {
			buffer[(j*WIDTH + i)*BPP + 0] = (i + j) * 128 / (WIDTH + HEIGHT); /* red */
			buffer[(j*WIDTH + i)*BPP + 1] = i * 128 / WIDTH; /* green */
			buffer[(j*WIDTH + i)*BPP + 2] = j * 256 / HEIGHT; /* blue */
		}
		buffer[j*WIDTH*BPP + 0] = 0xff;
		buffer[j*WIDTH*BPP + 1] = 0xff;
		buffer[j*WIDTH*BPP + 2] = 0xff;
		buffer[j*WIDTH*BPP + 3] = 0xff;
	}
}

static void newframebuffer(rfbScreenInfoPtr screen, int width, int height)
{
	unsigned char *oldfb, *newfb;

	oldfb = (unsigned char*)screen->frameBuffer;
	newfb = (unsigned char*)malloc(WIDTH * HEIGHT * BPP);
	initBuffer(newfb);
	rfbNewFramebuffer(screen, (char*)newfb, WIDTH, HEIGHT, 8, 3, BPP);
	free(oldfb);

	/*** FIXME: Re-install cursor. ***/
}


static void drawline(unsigned char* buffer, int rowstride, int bpp, int x1, int y1, int x2, int y2)
{
	int i, j;
	i = x1 - x2; j = y1 - y2;
	if (i == 0 && j == 0) {
		for (i = 0; i<bpp; i++)
			buffer[y1*rowstride + x1*bpp + i] = 0xff;
		return;
	}
	if (i<0) i = -i;
	if (j<0) j = -j;
	if (i<j) {
		if (y1>y2) { i = y2; y2 = y1; y1 = i; i = x2; x2 = x1; x1 = i; }
		for (j = y1; j <= y2; j++)
			for (i = 0; i<bpp; i++)
				buffer[j*rowstride + (x1 + (j - y1)*(x2 - x1) / (y2 - y1))*bpp + i] = 0xff;
	}
	else {
		if (x1>x2) { i = y2; y2 = y1; y1 = i; i = x2; x2 = x1; x1 = i; }
		for (i = x1; i <= x2; i++)
			for (j = 0; j<bpp; j++)
				buffer[(y1 + (i - x1)*(y2 - y1) / (x2 - x1))*rowstride + i*bpp + j] = 0xff;
	}
}


/* Here the pointer events are handled */

static void doptr(int buttonMask, int x, int y, rfbClientPtr cl)
{
	/*
	ClientData* cd = cl->clientData;

	if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT)
	{
		if (buttonMask) {
			int i, j, x1, x2, y1, y2;

			if (cd->oldButton == buttonMask) 
			{ // draw a line
				drawline((unsigned char*)cl->screen->frameBuffer, cl->screen->paddedWidthInBytes, BPP,
					x, y, cd->oldx, cd->oldy);
				x1 = x; y1 = y;
				if (x1>cd->oldx) x1++; else cd->oldx++;
				if (y1>cd->oldy) y1++; else cd->oldy++;
				rfbMarkRectAsModified(cl->screen, x, y, cd->oldx, cd->oldy);
			}
			else 
			{ // draw a point (diameter depends on button) 
				int w = cl->screen->paddedWidthInBytes;
				x1 = x - buttonMask; if (x1<0) x1 = 0;
				x2 = x + buttonMask; if (x2>WIDTH) x2 = WIDTH;
				y1 = y - buttonMask; if (y1<0) y1 = 0;
				y2 = y + buttonMask; if (y2>HEIGHT) y2 = HEIGHT;

				for (i = x1*BPP; i<x2*BPP; i++)
					for (j = y1; j<y2; j++)
						cl->screen->frameBuffer[j*w + i] = (char)0xff;
				rfbMarkRectAsModified(cl->screen, x1, y1, x2, y2);
			}

			// we could get a selection like that:
			// rfbGotXCutText(cl->screen,"Hallo",5);
			//
		}

		cd->oldx = x; cd->oldy = y; cd->oldButton = buttonMask;
	}
	*/

	std::srand(std::time(nullptr)); // use current time as seed for random generator
	int w = std::rand() / (RAND_MAX / 300);
	int h = std::rand() / (RAND_MAX / 300);
	drawline((unsigned char*)cl->screen->frameBuffer, cl->screen->paddedWidthInBytes, BPP,
		100, 100, 100 + w, 100 + h);

	rfbMarkRectAsModified(cl->screen, 0, 0, WIDTH, HEIGHT);
	rfbDefaultPtrAddEvent(buttonMask, x, y, cl);
}

static enum rfbNewClientAction newclient(rfbClientPtr cl)
{
	//cl->clientData = (void*)calloc(sizeof(ClientData), 1);
	//cl->clientGoneHook = clientgone;
	return RFB_CLIENT_ACCEPT;
}


int main(int argc,char** argv)
{
	rfbScreenInfoPtr screen = rfbGetScreen(&argc, argv, WIDTH, HEIGHT, 8, 3, BPP);
	if (!screen)
		return -1;
	screen->frameBuffer = static_cast<char*>(malloc(WIDTH * HEIGHT * BPP));
	screen->desktopName = "ARM";
	screen->alwaysShared = TRUE;
	screen->ptrAddEvent = doptr;
	
	initBuffer((unsigned char*)screen->frameBuffer);

	rfbInitServer(screen);
	rfbRunEventLoop(screen, 4000, FALSE);
	
	return(0);
}