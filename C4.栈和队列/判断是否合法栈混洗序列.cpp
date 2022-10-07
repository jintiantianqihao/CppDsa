#include <iostream>
#include <string>
#include "stack.hpp"
#include "queue.hpp"

using namespace std;

//栈混洗模拟——————判断序列是否为合法栈混洗序列

void stackPermutation(int B[], int size); //模拟栈混洗

int main() 
{
  int A[] = {3,2,1};
  stackPermutation(A, end(A) - begin(A));
}

void stackPermutation(int B[], int size) { //O(n)
  Stack<int> s; //辅助中转栈

  int i = 1; //模拟输入栈

  for (int k = 0; k < size; ++k) {
    while (s.empty() || B[k] != s.top()) {
      s.push(i++);
    }
    s.pop();
  }
}
//若合法，则正常返回0，无输出
//若非法，则内存泄漏