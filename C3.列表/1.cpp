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
  print(l);

  l.deduplicate();

  print(l);
  l.sort();
  print(l);

  cout << l.find(2)   << endl;
  l.sort();
  
  cout << l.find(8)   << endl;
  cout << l.search(4) << endl;
  cout << l.search(9) << endl;

}