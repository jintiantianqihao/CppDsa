#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include "stack.hpp"
#include "queue.hpp"

//表达式求值————逆波兰表达式
using namespace std;

double infix2postfix(char *S, char *RPN);      //中缀表达式转后缀
bool isDigit(char RPN);                        //操作数判断
void readNumber(Stack<double> &str, char *&p); //读数
double rpnEvaluate(char *RPN);                 //计算式
double calcu(double a, char op, double b);     //二元运算
double calcu(char op, double b);               //一元运算
int facI(int n);                               //阶乘

void append(char *&rpn, double operand); //操作数衔接
void append(char *&rpn, char op);        //操作符衔接


int main()
{
  string s;
  cout << "请输入你要计算的表达式：" << endl;
  getline(cin, s);
  
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
    else { //操作符情况
      if (*RPN == '!')
        operand.push(calcu(*RPN, operand.pop()));
      else {
        double b = operand.pop();
        double a = operand.pop();
        operand.push(calcu(a, *RPN, b));
      }
    }
  }

  return operand.top();
}

//操作数衔接
void append(char *&rpn, double operand) {
  int n = strlen(rpn); //RPN当前长度
  char buf[64];
  if (operand != (float)(int) operand) sprintf(buf, "%.2f \0", operand); //浮点格式
  else                                 sprintf(buf,"%d \0",(int)operand);//整数格式

  
}

//操作符衔接
void append(char *&rpn, char op) {
  
}

//中缀表达式转后缀
double infix2postfix(char *S, char *RPN) {

}