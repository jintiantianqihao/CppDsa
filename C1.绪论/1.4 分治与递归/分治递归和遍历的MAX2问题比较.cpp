#include <iostream>

using namespace std;

void swap(int &x1, int &x2)
{
  int temp = x1;
  x1 = x2;
  x2 = temp;
  //引用只会改变所绑定对象的值，不用引用就是形换神不换
  return;
}

/*  
 * 《遍历迭代法》
 *  最好情况：O(n-2 + 1) = O(n-1) = O(n)
 *  最坏情况：O(1 + n-2 + n-2) = O(2n-3) = O(n)
 */
void max2(int A[], int low, int high, int &x1, int &x2)
{
  if (A[x1 = low] < A[x2 = low +1]) swap(x1,x2);//O(1)

  for (int i = low + 2; i < high; ++i)
    if (A[x2] < A[i]) //O(1)
      if (A[x1] < A[x2 = i])//O(1)
        swap(x1,x2);
  
  return;  
}

/*
 *  《二分递归法》————分治
 *  最好情况：O(3/2 * n -2)
 *  最坏情况：O(5/3 * n -2)
 */
void max3(int A[], int low, int high, int &x1, int &x2)//high是尾后下标
{
  if (low + 2 == high) {
    if (A[low] > A[low + 1]) {
      x1 = low;
      x2 = low + 1;
    } else {
      x1 = low + 1;
      x2 = low;
    }
    return;
  }//T(2) = 1

  if (low + 3 == high) {
    if (A[low] > A[low + 1]) {
      if (A[low] > A[low + 2]) {  
        x1 = low;
        x2 = (A[low + 1] > A[low + 2]) ? low + 1 : low + 2;
      } else {
        x1 = low + 2;
        x2 = low;
      }
    } else {
      if (A[low + 1] > A[low + 2]) {  
        x1 = low + 1;
        x2 = (A[low] > A[low + 2]) ? low : low + 2;
      } else {
        x1 = low + 2;
        x2 = low + 1;
      }
    }
    
    return;
  }//T(3) <= 3

  int mid = low + ((high - low) >> 1); //divide
  int x1L, x2L; max3(A, low, mid, x1L, x2L);
  int x1R, x2R; max3(A, mid, high, x1R, x2R);

  if (A[x1L] > A[x1R]) {
    x1 = x1L;
    x2 = (A[x2L] > A[x1R]) ? x2L : x1R; 
  } else {
    x1 = x1R;
    x2 = (A[x2R] > A[x1L]) ? x2R : x1L;
  }// 1+1 = 2
  
  return;  
}//T(n) = 2*T(n/2) + 2  

//执行主函数
int main()
{
  int array[] = {0,5,4,7,8,9,1,2,3,2,2};
  int ptr[2];
  int &x1 = ptr[0];
  int &x2 = ptr[1];

  max2(array, 0, end(array) - begin(array), x1, x2);
  cout << array[x1] << " " << array[x2] << endl;

  max3(array, 0, end(array) - begin(array), x1, x2);
  cout << array[x1] << " " << array[x2] << endl;

  return 0;
}

