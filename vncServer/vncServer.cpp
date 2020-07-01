//#define snprintf _snprintf
/*
#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#else
#include <stdio.h> //sprintf
#endif
*/

#include <rfb/rfb.h>

#include "a.h" // DANGER will not be defined from here


const int WIDTH = 1600;
const int HEIGHT = 900;

/*
void just_segfault() {
  A a;
  // segmentation fault on 'a' destructor
}

void verbose_segfault() {
  A *a = new A();
  delete a;
}
/**/

int main(int argc,char** argv)
{
  /*
  std::cout << "sizeof(A) in main.cpp = " << sizeof(A) << std::endl;
  // verbose_segfault(); // uncomment this 
  just_segfault();
  std::cout << "This line won't be printed" << std::endl;
  /**/

  rfbScreenInfoPtr screen=rfbGetScreen(&argc,argv, WIDTH, HEIGHT,8,3,4);
  screen->frameBuffer = static_cast<char*>(malloc(WIDTH * HEIGHT * 4));
  rfbInitServer(screen);
  rfbRunEventLoop(screen,-1,FALSE);
  rfbMarkRectAsModified(screen, 0, 0, WIDTH, HEIGHT);
  return(0);
}