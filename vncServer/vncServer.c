#include <rfb/rfb.h>

const int WIDTH = 1600;
const int HEIGHT = 900;

int main(int argc,char** argv)
{
  rfbScreenInfoPtr screen=rfbGetScreen(&argc,argv, WIDTH, HEIGHT,8,3,4);
  screen->frameBuffer=malloc(WIDTH * HEIGHT * 4);
  rfbInitServer(screen);
  rfbRunEventLoop(screen,-1,FALSE);
  rfbMarkRectAsModified(screen, 0, 0, WIDTH, HEIGHT);
  return(0);
}