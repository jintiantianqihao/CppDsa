#include <iostream>
#include "list.hpp"

using namespace std;

int main()
{
  List<int> l;
  if (l.empty())
    for (Rank i = 0; i < 5; ++i) {
      l.insertAsLast(5 - i);
      l.insertAsLast(5 - i);
    }
  
  print(l);
  l.sort();
  l.deduplicate();
  print(l);

  auto pt = l.find(4);

  cout << pt << endl;


}