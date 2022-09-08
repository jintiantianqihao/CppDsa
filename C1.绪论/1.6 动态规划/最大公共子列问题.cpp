#include <iostream>
#include <string>

using namespace std;

using Length = unsigned;

int myMax(int a, int b)
{
  return ( a > b) ? a : b;
}

//只读类型
Length myLCS (const char *A, Length n, const char *B, Length m)
{
  if( n == 0 || m == 0) //平凡情况
    return 0;
  else if(*(A + n - 1) == *(B + m - 1))//减而治之
    return 1 + myLCS(A, n - 1, B, m - 1);
  else
    return max(myLCS(A, n, B, m - 1) , myLCS(A, n - 1, B, m));//分而治之
}

int main(void)
{
  string str1 ("program");
  string str2 ("algorithm");
  const char *A = str1.c_str();
  const char *B = str2.c_str();

  cout << myLCS(A, str1.size(), B, str2.size()) <<endl;

  return 0;
}