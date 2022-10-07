#include <iostream>
#include <string>
#include "stack.hpp"
#include "queue.hpp"

using namespace std;

//逆序输出

void convert(Stack<char> &s, int64_t n, unsigned base); //十进制数转换为任意进制数
void print(Stack<char> &s); //输出转换后的10进制数

int main() 
{
  Stack<char> s;
  int n, base;
  cout << "请输入一个10进制的数字和你想转换到的目标进制：";
  cin >> n >> base;

  convert(s, n, base);
  cout << n << "(10) = ";
  print(s);
  cout << "(" << base << ")" << endl;
}

void convert(Stack<char> &s, int64_t n, unsigned base) {
  //char digit[] = "0123456789ABCDEF"; //可扩充的进制数位值,有终止位'\0'
  //char digit[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; //没有终止位
  string digit = "0123456789ABCDEF";

  while (n > 0) { //短除处理过程，正向
    s.push(digit[n % base]);
    n /= base;
  }
}

void print(Stack<char> &s) {
  while (!s.empty()) {
    cout << s.pop();
  }
}