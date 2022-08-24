#include <iostream>
using namespace std;

typedef unsigned Positive;//非负数化实义命名

//Ackerman 函数实现
Positive myAckerman(Positive m, Positive n)
{
  if(m == 0)
    return n + 1;
  else if(n == 0)
    return myAckerman(m - 1 , 1);
  else 
    return myAckerman(m - 1 , myAckerman(m , n - 1));
}

//主函数：阿克曼函数
int main(void)
{
  int n = 0;
  cout<<"请输入计算值：";
  cin >> n;

  cout << "Ackerman("<< n << "," << n <<") = "<<myAckerman(n,n);

  return 0;
}