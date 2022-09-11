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
  _elem = new T[_capacity / 2]; //移位法扩大一倍
  for (Rank i = 0; i < _size; ++i) {
    _elem[i] = oldElem[i];
  }
  delete[] oldElem; //*释放原有空间

  return;
} //得益于向量封装，尽管扩容后数据区的物理地址有所改变，*但不会出现"野指针"

/*3.下标运算符重载：为了兼容原有运算符————循秩访问*/
template <typename T> T& Vector<T>::operator[] (Rank r) { //重载下标运算符
  return _elem[r]; //0 <= r <= _size
}

template <typename T> const T& Vector<T>::operator[](Rank r) const { //只读仅仅做右值的重载下标符
  return _elem[r]; //0 <= r <= _size
}

/*4.插入函数*/
template <typename T> Rank Vector<T>::insert(Rank r, T const &e) { // 0<=r<=_size
  expand(); //如有必要则扩容

  for (Rank i = _size; i > r; --i) { //O(n - r)：从后往前
    _elem[i] = _elem[i - 1];
  }
  _elem[r] = e; //插入新元素

  ++_size; //容量更新
  return r; //返回秩
}

/*5.删除函数*/
//缩容函数
template <typename T> void Vector<T>::shrink() { //装填因子过小时压缩空间，提高空间利用效率
  if (_capacity < DEFAULT_CAPACITY / 2) return; //保证不收缩到默认容量之下
  if (_size << 2 > _capacity) return; //保证装填因子_size/_capacity在25%以下才进行缩容

  T *oldElem = _elem; //另开辟新的动态内存空间
  _elem = new T[_capacity / 2]; //容量减半
  for (Rank i = 0; i < _size; ++i) {// O(n)
    _elem[i] = oldElem[i];
  }
  delete [] oldElem; //释放原空间
  
  return;
}

//区间删除
template <typename T> int Vector<T>::remove(Rank low, Rank high) { //O(n -high)
  if (low == high) return 0; //为了提升算法效率，单独处理特殊简易的退化情况
  while (high < _size) {
    _elem[low++] = _elem[high++]; //O(high - low) : [high,_size)顺次前移，low更新到low + _size - high = n - (high - low)
  }
  _size = low; //更新规模
  shrink(); //如有必要则缩容,都是有this指针的

  return high - low; //返回被删除元素个数  
}

//单元素删除：直接调用区间删除的特例版即可
template <typename T> T Vector<T>::remove(Rank r) { //O(n-r)
  T e = _elem[r]; //备份，用作返回值
  remove(r, r + 1);
  return e; //返回被删除的元素
}//若反之利用递归调用单元素删除实现区间删除，则会遭遇与循环移位相同的窘境：大量单次无效的前移，不能一步到位，从而导致时间复杂度为O(n^2)而非O(n)

/*6.查找函数*/
template <typename T> Rank Vector<T>::find(T const &e, Rank low, Rank high) const { //O(high - low), 0<=low<=high<=_size
  while ((low < high--) && (e != _elem[high]));//while ((e != _elem[--high]) && (low <= high)); (错误，可能在最后越下界访问到_elem[-1],发生段错误)
  return high; //返回值小于low时则查找失败；否则命中目标的秩，多值情况返回秩最大者  
}

/*7.无序去重函数*/
template <typename T> Rank Vector<T>::deduplicate() {
  Rank oldSize = _size;
  for (Rank i = 1; i < _size; ) {
    (find(_elem[i], 0, i) < 0) ? ++i : remove(i); //前缀钟寻找雷同者；没找到，继续找下一个；找到，删除该重复元素，并更新规模
  }
  
  return oldSize - _size; //返回所有重复元素的个数  
}

/*8.遍历函数————利用函数对象实现*/
//函数指针实现法
template <typename T> void Vector<T>::traverse(void (*visit) (T&)) { /*返回值+函数名+参数列表类型*/
  for(Rank i = 0; i < _size; ++i) {
    visit(_elem[i]);
  }
  return;
} //函数指针，只读类型或者做局部修改

//函数对象模版实现法
template <typename T> template <typename VST> void Vector<T>::traverse(VST visit) { //函数对象，便于全局修改
  for (Rank i = 0; i < _size; ++i) {
    visit( _elem[i] );
  }
  return;
}

/* 测试函数
template <typename T> struct myPrint { virtual void operator()(T &e) { std::cout << e <<" ";}};//函数对象，单个打印：通过重载操作符()实现
template <typename T> void print(Vector<T> &v) { v.traverse( myPrint<T>() ); std::cout << std::endl;};//向量遍历打印
template <typename T> struct myIncrease { virtual void operator()(T &e) { ++e;}};//函数对象，单个加一：通过重载操作符()实现
template <typename T> void increase(Vector<T> &v) { v.traverse( myIncrease<T>() );};//向量遍历加一
template <typename T> struct myDecrease { virtual void operator()(T &e) { --e;}};//函数对象，单个减一：通过重载操作符()实现
template <typename T> void decrease(Vector<T> &v) { v.traverse( myDecrease<T>() );};//向量遍历减一
template <typename T> struct myDoubble { virtual void operator()(T &e) { e *= 2;}};//函数对象，单个加倍：通过重载操作符()实现
template <typename T> void doubble(Vector<T> &v) { v.traverse( myDoubble<T>() );};//向量遍历加倍
template <typename T> struct mySum { virtual void operator()(T &e) { sumValue += e;}};//通过全局变量记录求和值
template <typename T> void sum(Vector<T> &v) { v.traverse( mySum<T>() );};//向量整体求和
*/

