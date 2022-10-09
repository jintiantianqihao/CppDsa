#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <cctype>
#include "priority.h"
#include "stack.hpp"
#include "queue.hpp"

//表达式求值————逆波兰表达式
using namespace std;

void infix2postfix(char *S, char *RPN, int &size); //中缀表达式转后缀
bool isDigit(char RPN);                           //操作数判断
void readNumber(Stack<double> &str, char *&p);    //读数
double rpnEvaluate(char *RPN);                    //计算式
double calcu(double a, char op, double b);        //二元运算
double calcu(char op, double b);                  //一元运算
int facI(int n);                                  //阶乘

char *removeSpace(char *s);                         //输入式空格排除
void append(char *&rpn, double operand, int &size); //操作数衔接
void append(char *&rpn, char op, int &size);        //操作符衔接
char orderBetween(char op1, char op2);              //运算符优先级比较


int main()
{
  string s;
  cout << "请输入你要计算的表达式：" << endl;
  getline(cin, s);

  //int size = s.length();
  //char* rpn = (char*) malloc(sizeof(char) * size);   rpn[0] = '\0'; //逆波兰表达式
  const char* str = s.c_str(); //返回一个C风格字符串
  //infix2postfix(removeSpace(const_cast<char*>(str)), rpn, size); //做了一个强制转换
  cout << rpnEvaluate(const_cast<char*>(str)) << endl;
  //free(rpn);
  //rpn = nullptr;

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
void readNumber(Stack<double> &stk, char *&p) {
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
double rpnEvaluate(char *RPN) {
  Stack<double> operand;

  while (*RPN != '\0') {
    if (isDigit(*RPN))
      readNumber(operand, RPN);
    else if (isspace(*RPN))
      ++RPN;
    else { //操作符情况
      if (*RPN == '!')
        operand.push(calcu(*RPN, operand.pop()));
      else {
        double b = operand.pop();
        double a = operand.pop();
        operand.push(calcu(a, *RPN, b));
      }
      ++RPN;
    }
  }

  return operand.top();
}

//操作数衔接
void append(char *&rpn, double operand, int &size) {
  int n = strlen(rpn); // RPN当前长度
  char buf[64];
  if (operand > (int)operand)
    sprintf(buf, "%.2f \0", operand); //浮点格式 (自主赋了空格)
  else
    sprintf(buf, "%d \0", (int)operand); //整数格式

  if (n + strlen(buf) + 1 > size)
    rpn = (char *)realloc(rpn, sizeof(char) * (strlen(rpn) + strlen(buf) + 1)); //动态扩容

  strcat(rpn, buf); // RPN增长
}

//操作符衔接
void append(char *&rpn, char op, int &size) {
  int n = strlen(rpn);
  if (n + 3 > size)                        //此处3包含三部分：空格符+符号+'\0'
    rpn = (char *)realloc(rpn, size *= 2); //动态扩容

  sprintf(rpn + n, "%c ", op); //接入指定运算符
  rpn[n + 2] = '\0';
}

//***********************中缀表达式转后缀*****************************//
void infix2postfix(char *S, char *RPN, int &size) { // O(n)
  Stack<double> operand;                           //操作数栈
  Stack<char> optr;                                //操作符栈
  char *expr = S;
  optr.push('\0');

  while (!optr.empty()) {
    if (isDigit(*S)) { //若为操作数
      readNumber(operand, S);
      append(RPN, operand.top(), size);        //读入操作数，并将其接至RPN末尾
    } else {                                   //若当前字符为运算符
      switch (orderBetween(optr.top(), *S)) //当前运算符和栈顶运算符比较优先级
      {
        case '<': { //栈顶优先级低时
          optr.push(*S);
          ++S;
          break; //推迟计算，栈顶运算符入栈
        }
        case '=': { //优先级相同（右括号和'\0'结束时）
          optr.pop();
          if(optr.top() != '\0') ++S;
          break; //脱括号并进入下一步
        }
        case '>':                        //栈顶优先级更高时，可进行计算，将要进行计算的栈顶运算符衔接到RPN尾部
          append(RPN, optr.pop(), size); //栈顶运算符出栈并衔接在rpn尾部
          break;
        default:
          exit(-1); //遇到语法错误，不做处理直接退出
          break;
      }
    }
  }
}
//*******************************************************************//

//空格除去
char *removeSpace(char *s) {
  char *p = s, *q = s;
  while (true) {
    while (isspace(*q)) //绕过空格
      ++q;
    if ((*q) == '\0') {
      *p = '\0';
      return s;
    }
    *(p++) = *(q++); // q为快指针，p为慢指针
  }
}

//运算符优先级比较
char orderBetween(char op1, char op2) {
  return pri[op2rank(op1)][op2rank(op2)];
}