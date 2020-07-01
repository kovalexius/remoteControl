#ifndef __A__H
#define __A__H

#include <string>
#include <vector>
#include <iostream>

struct Fill
{
  //*
  Fill()
  {
    ptr1 = 3;
  }
  ~Fill()
  {
    ptr1 = 0;
  }
  /**/
  int ptr1;
  int ptr2;
  int ptr3;
};

class A 
{
public:
  A(); // implementation in a.cpp with DANGER defined
  ~A(); // for illustrational purposes
#ifdef DANGER
  Fill m_fill;
#endif // DANGER
};

#endif