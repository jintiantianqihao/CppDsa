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

//主函数：计算反阿克曼函数
int main(void)
{
  int n = 0;//初始阿克曼输入值

  cout << "请输入你的计算规模：" << endl;
  cin >> n;

  Positive antiAckerman = 1;//反Ack值初始化 A(0,0) = 1;
  //反阿克曼求解
  Positive cnt = 1;
  while (myAckerman(cnt,cnt) < n)
  {
    ++cnt;
  }
  cout<< "反阿克曼值antiAckerman (" << n <<") = "<< cnt << endl;

  return 0;
  
}