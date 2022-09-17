#include <iostream>
#include "vector.hpp"

using namespace std;

int main(void)
{
  int A[] = {7,3,4,2,1,9};
  Vector v1(A, end(A) - begin(A) - 3); //自己通过A的类型T*判断模板使用
  Vector v2(A, end(A) - begin(A)); //自己通过A的类型T*判断模板使用
  print(v2);
  v2.sort();
  print(v2);
  v2.insert(v2.search(8), 8);
  print(v2);
  print(v1);
  v1 = v2;
  print(v1);
  v1.unsort();
  print(v1);
  
}