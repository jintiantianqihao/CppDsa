#include <iostream>
using namespace std;
using Rank = int;

void mySwap(Rank &a, Rank &b) {
  Rank temp = a;
  a = b;
  b = temp;
}

void myReverse(Rank* A, Rank size) {
  Rank low = 0, high = size;

  while (low < high)
    mySwap(A[low++], A[--high]);
  
}

int main() {

  Rank n = 0, k = 0;
  cin >> n >> k;
  Rank A[n];
  for (size_t i = 0; i < n; ++i) {
    cin >> A[i];
  }
  
  myReverse(A, n);
  myReverse(A, k % n);
  myReverse(A + k % n, n - k % n);

  for (Rank i = 0; i < n; ++i) {
    (i == n-1) ? (cout << A[i]) : (cout << A[i] << " ");
  }
  
  return 0;
}