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

//同余移动k位
int shift(int *A, int n, int s, int k)//s是抽出的同余头
{
  int temp = A[s];//利用O(1)空间抽出同余节点
  int i = s, mov = 0;
  for (size_t j = (s + k) % n; s != j; ++mov)
  {
    A[i] = A[j];
    i = j;
    j = (j + k) % n;
  }
  A[i] = temp;

  return mov + 1;
}

//翻转函数
void myReverse(int *A, int k)
{
  for (int low = 0, high = k-1; low < high; ++low, --high)
  {
    int temp = A[low];
    A[low] = A[high];
    A[high] = temp;
  }
  
  return;
}

/*  
 *  method1."蛮力减治法：O(n*k)" 
 *  每次仅移动一位，反复多次位移k位
 */
void shift0(int *A, int n, int k)
{
  while ( k-- )
  {
    shift(A, n, 0, 1);//一位移动
  }
  
  return;
}

/*
 *  method2."同余迭代法：O(n + z)",z为最大公约数
 *  最大公约数迭代
 */
void shift1(int *A, int n, int k)
{
  for (int mov = 0, s = 0; mov < n; ++s)
  {
    mov += shift(A, n, s, k);
  }
}

/*
 *  method3."翻转法：O(3n)"
 *  三次捉对翻转————此法用到了缓存机制！！
 */
void shift2(int *A, int n, int k)
{
  myReverse(A, k);//O(3/2*k)
  myReverse(A + k, n - k);//O(3/2*(n - k))
  myReverse(A, n);//O(3/2*n)
}



int main(void)
{
  int A[] = {1,2,3,4,5,6,7};

  shift0(A, end(A) - begin(A), 3);
  myPrint(A, end(A) - begin(A));
  shift1(A, end(A) - begin(A), 4);
  myPrint(A, end(A) - begin(A));
  shift2(A, end(A) - begin(A), 3);
  myPrint(A, end(A) - begin(A));

  return 0;
}