#include <iostream>
using namespace std;

typedef unsigned Positive;//非负数化实义命名

//fib 函数实现
/*1.二分递归法 ： O(2^n) */
Positive fib1(Positive n)
{
  return (n < 2) ? n : fib1(n - 1) + fib1(n - 2);
}

/*2.线性迭代法：O(n)----"上台阶"*/
Positive fib2(Positive n)
{
  Positive f = 1, g = 0;
  if(n == 0) return g;
  --n;
  while ( n-- > 0)
  {
    f = f + g;
    g = f - g;
  }

  
  return f;
  
}

int main(void)
{
  int n = 0;
  cout << "请输入计算值：" << endl;
  cin >> n;

  for (size_t i = 0; i < n; ++i)
  {
    cout << "fib(" << i << ") = " << fib1(i) <<endl;
    //cout << "fib(" << i << ") = " << fib2(i) <<endl;
  }
  
  
  return 0;
} 