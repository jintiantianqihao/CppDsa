#include <iostream>
#include <string>
#include <cstring>

using namespace std;

typedef unsigned Length;

void mySwap(Length &x, Length &y)
{
  Length temp = x;
  x = y;
  y = x;

  return;
}

void mySwap(Length *x, Length *y)
{
  Length temp = *x;
  *x = *y;
  *y = temp;

  return;
}

void mySwap(const char **A, const char **B)
{
   const char *temp = *A;
   *A = *B;
   *B = temp;  

  return;
}

int myMax(int a, int b)
{
  return ( a > b) ? a : b;
}

//只读类型
Length myLCS (const char *A, Length n, const char *B, Length m)
{
  if(n < m) { mySwap(&A ,&B); mySwap(m ,n); }

  Length *lcs1 = new Length[m + 1];
  Length *lcs2 = new Length[m + 1];
  memset(lcs1, 0x00 ,sizeof(Length) * (m + 1));
  memset(lcs1, 0x00 ,sizeof(Length) * (m + 1));
  
  for (size_t i = 0; i < n; mySwap(lcs1,lcs2), i++)
  {
    for (size_t j = 0; j < m; ++j)
    {
      lcs2[j + 1] =  (A[i] == B[j]) ? 1+lcs1[j] : max(lcs2[j], lcs1[j + 1]);
    }
  }
  
  Length solu = lcs1[m];
  delete[] lcs1;
  delete[] lcs2;

  return solu;
}

int main(void)
{
  string str1 ("tsinghua");
  string str2 ("computer");
  const char *A = str1.c_str();
  const char *B = str2.c_str();

  cout << myLCS(A, str1.size(), B, str2.size()) <<endl;

  return 0;
}