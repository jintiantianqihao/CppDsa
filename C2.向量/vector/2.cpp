#include <iostream>
#include "vector.h"

int main()
{
  Vector<int> v1(4,2,2);
  print(v1);
  v1.remove(0);
  print(v1);
}