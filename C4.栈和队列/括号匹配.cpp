#include <iostream>
#include <string>
#include "stack.hpp"
#include "queue.hpp"

using namespace std;

bool CheckBalancedParentheses (const string& exp, int low, int high); //栈对于递归嵌套问题有着天然的优势

int main() 
{
  string str;
  cout << "请输入一串字符表达式" << endl;
  cin >> str;

  if (CheckBalancedParentheses(str, 0, str.size()))
    cout << "匹配" << endl;
  else
    cout << "不匹配" << endl;
}

bool CheckBalancedParentheses (const string& exp, int low, int high) { //O(n)：可以推广到任意匹配需求情况
  Stack<char> s;

  for (int i = low; i < high; ++i) {
    switch (exp[i]) {
      case '(':
      case '[':
      case '{':
        s.push(exp[i]);
        break;

      case ')':                           //此处if-else算一条语句
        if (!s.empty() && s.top() == '(') //相当于只要遇到右括号，不消除就必不平衡
          s.pop();
        else
          return false;
        break;

      case ']':
        if (!s.empty() && s.top() == '[')
          s.pop();
        else
          return false;
        break;

      case '}':
        if (!s.empty() && s.top() == '{')
          s.pop();
        else
          return false;
        break;

      default: //跳过非括号字符
        break;
    }
  }

  return s.empty();
}