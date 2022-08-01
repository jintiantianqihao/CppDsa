#include <iostream>

using std::cin,std::cout,std::endl;

//计算Hailstone序列长度
int hailstone(int n)
{
  int length = 1;//从1开始，以下按定义逐步递推序列，累计步数，直至n = 1;
  
  while (n > 1)
  {
    (n % 2) ? (n = 3*n + 1) : (n=n/2);//（n % 2） ? n = 3*n + 1 : n=n/2;可读性较差
    ++length;
  }

  return length;//返回|Hailstone(n)|即序列长度
}

int main(void)
{
  cout << "请输入一个整数："<< endl;
  int n;
  cin >> n;

  cout<<"Hailstone序列长度为："<< hailstone(n) <<endl;

  return 0;
}