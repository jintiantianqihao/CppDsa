#include <iostream>
#include "list.hpp"

using namespace std;

int main()
{
  List<int> l;
  if (l.empty())
    for (Rank i = 0; i < 7; ++i) {
      l.insertAsLast(7 - i);
    }
  print(l);


  l.sort();
  print(l);
}