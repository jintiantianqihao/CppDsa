#include <iostream>
#include <string>
#include <cmath>
#include "priority.h"
#include "stack.hpp"

//表达式求值————逆波兰表达式
using namespace std;
using constStrPtr = string::const_iterator; //迭代器类型传入

string infix2postfix(const string &str);  //中缀表达式转后缀
bool isDigit(char RPN);                              //操作数判断
void readNumber(Stack<double> &str, constStrPtr &p); //读数
double rpnEvaluate(const string &RPN);               //计算式
double calcu(double a, char op, double b);           //二元运算
double calcu(char op, double b);                     //一元运算
int facI(int n);                                     //阶乘

char orderBetween(const char &op1, const char &op2); //运算符优先级比较


int main()
{
  string s, rpn;
  cout << "请输入你要计算的表达式：" << endl;
  getline(cin, s);

  rpn = infix2postfix(s);
  cout << rpn << " = " << rpnEvaluate(rpn) << endl;

  return 0;
}

//二元运算
double calcu(double a, char op, double b) {
  switch (op) {
    case '+':
      return a + b;
      break;
    case '-':
      return a - b;
      break;
    case '*':
      return a * b;
      break;
    case '/':
      return ((b == 0) ? -1 : a / b);
      break;
    case '^':
      return pow(a, b);
      break;
    default:
      return -1;
      break;
  }
}

//一元运算
double calcu(char op, double b) {
  int result = -1;
  if (op == '!')
    result = facI(b);
  return result;
}

//阶乘
int facI(int n) {
  if (n == 0 || n == 1)
    return 1;
  else {
    int res = n;
    while (--n) {
      res *= n;
    }
  return res;
  }
}

//操作数判断
bool isDigit(char RPN) {
  return ((RPN >= '0' && RPN <= '9') ? true : false);
} 

//读数 
void readNumber(Stack<double> &stk, constStrPtr &p) {
  stk.push(static_cast<double>(*p - '0'));                    //当前数位入栈
  while (isDigit(*(++p)))                                     //后续数字紧邻
    stk.push(10 * stk.pop() + static_cast<double>(*p - '0')); //栈中数弹出进位，新数值重新入栈
  if ((*p) != '.') return;                                    //后位数非小数点，则以整数返回
  double fraction = 1;
  while (isDigit(*(++p))) { //小数点后数字紧邻
    fraction /= 10;
    stk.push(stk.pop() + (*p - '0') * fraction); //小数点后数字入栈
  }
}

//RPN计算
double rpnEvaluate(const string &RPN) {
  Stack<double> operand;
  auto beg = RPN.begin(); // auto关键字自动匹配类型

  while (beg != RPN.end()) {
    if (isDigit(*beg)) //操作数情况
      readNumber(operand, beg);
    else if (isspace(*beg))
      ++beg;
    else { //操作符情况
      if (*beg == '!')
        operand.push(calcu(*beg, operand.pop()));
      else {
        double b = operand.pop();
        double a = operand.pop();
        operand.push(calcu(a, *beg, b));
      }
      ++beg;
    }
  }

  return operand.top();
}

//***********************中缀表达式转后缀*****************************//
string infix2postfix(const string &str) { // O(n)
  Stack<double> operand;                             //操作数栈
  Stack<char> optr;                                  //操作符栈

  string rpn = "";
  auto beg = str.begin(); // auto自动匹配迭代器类型
  auto end = str.end();   // auto自动匹配迭代器类型

  while (beg != end) {
    if (isDigit(*beg) || (*beg == '.')) {        //若为操作数
      rpn += *beg++;              //读入操作数，并将其接至RPN末尾
    } else if (isspace(*beg)) { //当前字符为空格符
      ++beg;
    } else {                                  //若当前字符为运算符
      rpn += " ";                             //操作数终止，遇到操作符，则在操作数后面加空格以便于区分
      switch (orderBetween(optr.top(), *beg)) //当前运算符和栈顶运算符比较优先级
      {
        case '<': { //栈顶优先级低时
          optr.push(*beg);
          ++beg;
          break; //推迟计算，栈顶运算符入栈
        }
        case '=': { //优先级相同（右括号时）
          optr.pop();
          ++beg;
          break; //脱括号并进入下一步
        }
        case '>':
          rpn += optr.pop(); //栈顶优先级更高时，可进行计算，将要进行计算的栈顶运算符衔接到RPN尾部
          break;
        default:
          exit(-1); //遇到语法错误，不做处理直接退出
          break;
      }
    }
  }

  while (!optr.empty()) //遍历完字符串后，将符栈内操作符全数弹出
    rpn += optr.pop();

  return rpn;
}
//*******************************************************************//

//运算符优先级比较
char orderBetween(const char &op1, const char &op2) {
  return pri[op2rank(op1)][op2rank(op2)];
}