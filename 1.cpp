#include <iostream>

using namespace std;


void dice()
{
  int step = 0;

  while ( rand() & 1 )
  {
    ++step;
  }
  cout<<step;
}

int main()
{
  dice();

  return 0;
}