#include <iostream>
#include "vector.h"

/*1.复制函数*/
template <typename T> void Vector<T>::copyFrom( T const* A, Rank low, Rank high) { //T为基本类型或者重载后的操作符
  _elem = new T[_capacity = 2*(high - low)];//分配空间，并为_capacity赋值
  _size = 0;//大小初始化，规模清零
  while (low < high)
    _elem[_size++] = A[low++];//复制到_elem[0,high - low)

  return;
}

/*2.扩容函数*/
template <typename T> void Vector<T>::expand() { //向量空间不足时扩容
  if (_size < _capacity) return; //尚未满员时，不必扩容
  _capacity = myMax(_capacity, DEFAULT_CAPACITY); //往大处扩容

  T* oldElem = _elem;
  _elem = new T[_capacity << 1]; //移位法扩大一倍
  for (Rank i = 0; i < _size; ++i) {
    _elem[i] = oldElem[i];
  }
  delete[] oldElem; //*释放原有空间

  return;
} //得益于向量封装，尽管扩容后数据区的物理地址有所改变，*但不会出现"野指针"

/*3.下标运算符重载：为了兼容原有运算符*/
template <typename T> T& Vector<T>::operator[] (Rank r) { //重载下标运算符
  return _elem[r]; //0 <= r <= _size
}

template <typename T> const T& Vector<T>::operator[](Rank r) const { //只读仅仅做右值的重载下标符
  return _elem[r]; //0 <= r <= _size
}