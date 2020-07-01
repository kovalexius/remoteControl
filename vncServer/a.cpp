#include <iostream>

#define DANGER // let's have a situation
#include "a.h"

A::A() {
  std::cout << "sizeof(A) in A constructor = " << sizeof(A) << std::endl;
  std::cout << "sizeof *this in A constructor = " << sizeof *this << std::endl;
}

A::~A() {
  std::cout << "sizeof(A) in A destructor = " << sizeof(A) << std::endl;
  std::cout << "sizeof *this in A destructor = " << sizeof *this << std::endl;
  std::cout << "Segmentation fault incoming..." << std::endl;
}