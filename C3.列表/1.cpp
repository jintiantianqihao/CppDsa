#include <iostream>
#include "list.hpp"

using namespace std;

int main()
{
  List<int> l;
  int size = 0;
  cin >> size;
  if (l.empty())
    for (Rank i = 0; i < size; ++i)
      l.insertAsLast(i);

  print(l);
  cout << l[l.size() % 2] << endl;
  l.insertAsLast(7);
  List<int> l2(l, 0, l.size() - 1);
  print(l);
  print(l2);
  l2.remove(l2.first()->succ());
  l2.insertAsFirst(0);
  l2.insertAsFirst(1);

  if (l2.find(4)) {
    cout << "删除后的链表：";
    print(l2);
  }
  if (l2.deduplicate()) {
    cout << "去重后的链表：";
    print(l2);
  }
}