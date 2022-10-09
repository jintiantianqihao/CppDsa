#include<iostream>
#include<stack>
#include<string>
#include<cmath>


using namespace std;

// Function to convert Infix expression to postfix 
string InfixToPostfix(const string &expression);

// Function to verify whether an operator has higher precedence over other
int HasHigherPrecedence(char operator1, char operator2);

// Function to verify whether a character is operator symbol or not. 
bool IsOperator(char C);

// Function to verify whether a character is alphanumeric chanaracter (letter or numeric digit) or not. 
bool IsOperand(char C);

//运算函数
double rpnEvaluate(const string &RPN);     //计算式
double calcu(double a, char op, double b); //二元运算
double calcu(char op, double b);           //一元运算
int facI(int n);                           //阶乘


int main() 
{
	string expression; 
	cout<<"Enter Infix Expression \n";
	getline(cin,expression);
	string postfix = InfixToPostfix(expression);
	cout<<"Output = "<<postfix<<"\n";

  cout << expression << " = " << rpnEvaluate(postfix) << endl;
}

// Function to evaluate Postfix expression and return output
string InfixToPostfix(const string &expression) {
  // Declaring a Stack from Standard template library in C++.
  stack<char> S;
  string postfix = ""; // Initialize postfix as empty string.
  for (int i = 0; i < expression.length(); i++) {

    // Scanning each character from left.
    // If character is a delimitter, move on.
    if (expression[i] == ' ' || expression[i] == ',') continue;

    // If character is operator, pop two elements from stack, perform operation and push the result back.
    else if (IsOperator(expression[i])) {
			postfix += " ";
      while (!S.empty() && S.top() != '(' && HasHigherPrecedence(S.top(), expression[i])) {
        postfix += S.top();
        S.pop();
      }
      S.push(expression[i]);
    }
    // Else if character is an operand
    else if (IsOperand(expression[i]) || expression[i] == '.') {
      postfix += expression[i];
    }

    else if (expression[i] == '(') {
      S.push(expression[i]);
    }

    else if (expression[i] == ')') {
      while (!S.empty() && S.top() != '(') {
        postfix += S.top();
        S.pop();
      }
      S.pop();
    }
  }

  while(!S.empty()) {
		postfix += S.top();
		S.pop();
	}

	return postfix;
}

// Function to verify whether a character is english letter or numeric digit.
// We are assuming in this solution that operand will be a single character
bool IsOperand(char C) {
  if (C >= '0' && C <= '9') return true;
  return false;
}

// Function to verify whether a character is operator symbol or not.
bool IsOperator(char C) {
  if (C == '+' || C == '-' || C == '*' || C == '/' || C == '^' || C == '!')
    return true;

  return false;
}

// Function to verify whether an operator is right associative or not.
int IsRightAssociative(char op) {
  if (op == '^') return true;
  return false;
}

// Function to get weight of an operator. An operator with higher weight will have higher precedence.
int GetOperatorWeight(char op) {
  int weight = -1;
  switch (op) {
    case '+':
    case '-':
      weight = 1;
    case '*':
    case '/':
      weight = 2;
    case '^':
      weight = 3;
    case '!':
      weight = 4;
  }
  return weight;
}

// Function to perform an operation and return output. 
int HasHigherPrecedence(char op1, char op2)
{
	int op1Weight = GetOperatorWeight(op1);
	int op2Weight = GetOperatorWeight(op2);

	// If operators have equal precedence, return true if they are left associative. 
	// return false, if right associative. 
	// if operator is left-associative, left one should be given priority. 
	if(op1Weight == op2Weight)
	{
		if(IsRightAssociative(op1)) return false;
		else return true;
	}
	return op1Weight > op2Weight ?  true: false;
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


// RPN计算
double rpnEvaluate(const string &RPN) {
  stack<double> operand;
  auto begin = RPN.begin();
  auto end = RPN.end();

  while (begin != end) {
    if (IsOperand(*begin)) {                           //操作数读入————readNumber()---传入迭代器的引用即可
      operand.push(static_cast<double>(*begin - '0')); //当前数位入栈
      while (IsOperand(*(++begin))) {
        double temp = 10 * operand.top() + static_cast<double>(*begin - '0');
        operand.pop();
        operand.push(temp);          //栈中数弹出进位，新数值重新入栈
      }                              //后续数字紧邻
      if ((*begin) != '.') continue; //后位数非小数点，则以整数返回
      double fraction = 1;
      while (IsOperand(*(++begin))) { //小数点后数字紧邻
        fraction /= 10;
        double temp = operand.top() + (*begin - '0') * fraction;
        operand.pop();
        operand.push(temp); //小数点后数字入栈
      }
    } else if (IsOperator(*begin)) { //操作符情况
      if (*begin == '!') {
        double temp = calcu(*begin, operand.top());
        operand.pop();
        operand.push(temp);
      } else {
        double b = operand.top();
        operand.pop();
        double a = operand.top();
        operand.pop();
        operand.push(calcu(a, *begin, b));
      }
      ++begin;
    } else //跳过非法空字符情况
      ++begin;
  }

  double res = operand.top();
  operand.pop();
  if (operand.empty())
    return res;
  else {
    cout << "ERROR" << endl;
    return -1;
  }
  // return operand.top();
}