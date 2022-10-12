#include <iostream>
//动态内存

const int N = 5e5+10;
int array[N];
int n, m;

int searchMax(int const &e);
int searchMin(int const &e);
void mergeSort(int low, int high); //归并排序
void merge(int low, int mid, int high); //二路归并

int main(void) 
{
  int a = 0, b = 0;
  scanf("%d %d", &n, &m);

  for (size_t i = 0; i < n; ++i)
    scanf("%d", &array[i]);

  mergeSort(0, n);

  while (m--) {
    scanf("%d %d", &a, &b);
    
    int min = searchMin(a);
    int max = searchMax(b);

    printf("%d\n", max - min + 1); //二者均命中的情况
  }

  return 0;
}

int searchMax(int const &e) {
  int low = 0;
  int high = n;
  while (low < high) {
    int mid = low + ((high - low) >> 1);
    if (e < array[mid])
      high = mid; //搜索空间：[low, mid)
    else
      low = mid + 1; //搜索空间：(mid, high)
    //看起来mid被空，但可以通过返回值修正
  }
  return low - 1; //返回不大于查找值e的最大值

  //后续可以出来后根据下标和与e判等操作，看是否找到
}

int searchMin(int const &e) {
  int low = 0;
  int high = n;
  while (low < high) {
    int mid = low + ((high - low) >> 1);
    if (e <= array[mid])
      high = mid; //搜索空间：[low, mid)
    else
      low = mid + 1; //搜索空间：(mid, high)
    //看起来mid被空，但可以通过返回值修正
  }
  return high; //返回不大于查找值e的最小值

  //后续可以出来后根据下标和与e判等操作，看是否找到
}

//归并排序算法接口（递归）
void mergeSort(int low, int high) { // O(nlogn)
  if (high - low == 1) //递归基（递归车起步，先系安全带）
    return;

  int mid = low + ((high - low) >> 1);
  mergeSort(low, mid);  //[low,mid)
  mergeSort(mid, high); //[)
  merge(low, mid, high);

  return;
}

//二路归并
void merge(int low, int mid, int high) { // O(n)
  int *A = array + low;
  int i = 0; // A就地
  int lb = mid - low;

  int *B = new int[lb];
  int j = 0; // B申请堆空间
  for (int k = 0; k < mid - low; ++k) {
    B[k] = A[k];
  } //逐个拷贝B

  int lc = high - mid;
  int *C = array + mid;
  int k = 0; // C就地

  while ((j < lb) && (k < lc)) {              //一般情况
    A[i++] = (B[j] > C[k]) ? C[k++] : B[j++]; //保证排序稳定性，相等时需要返回左侧的值
  }
  while (j < lb) { // C先排尽，还要排B；B先排尽，C可顺序就位
    A[i++] = B[j++];
  }
  delete[] B; // new和delete很耗时，可用内置栈结构来代替

  return;
}


