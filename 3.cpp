#include <iostream>
using namespace std;

void myPrint(int *A, int n)
{
  for (size_t i = 0; i < n; ++i)
  {
    cout << *(A + i) << " ";
  }
  cout << endl;

  return;  
}

/*  
 *  method1."减治法：O(n*k)" 
 *  每次仅移动一位，反复多次位移k位
 */

//只移动一位
void shift(int *A, int n, int end, int k)
{
  int temp = A[0];
  for (size_t i = 0; i < n - 1 ; ++i)
  {
    A[i] = A[i + 1];
  }
  A[n - 1] = temp;

  return;
}

//反复一位移动（减而治之）
void shift0(int *A, int n, int k)
{
  while ( k-- )
  {
    shift(A, n, 0, 1);//一位移动
  }
  
  return;
}

/*
 *  method2.""
 *  
 */



int main(void)
{
  int A[] = {1,2,3,4,5,6,7};


  myPrint(A, end(A) - begin(A));
  shift0(A, end(A) - begin(A), 5);
  myPrint(A, end(A) - begin(A));

  return 0;
}