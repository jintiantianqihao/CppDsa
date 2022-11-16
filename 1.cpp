#include <iostream>
using namespace std;

//辅助函数
int max(int const &a,int const &b) {
  return (a > b) ? a : b;
}

int main(void) {
  int i, j;

  cin >> i >> j;
  cout << max(i, j) << endl;

  return 0;
}