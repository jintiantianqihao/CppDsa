#ifndef STACK_H
#define SRACK_H

#include "vector.hpp" //以向量为基类，派生出栈模板类

template <typename T>
class Stack : public Vector<T> {
public:                                              //沿用原有接口：如_size
  void push(T const &e) { this->insert(e); }         //入栈，默认末端插入
  T pop() { return this->remove(this->size() - 1); } //出栈，末端删除元素
  T &top() { return (*this)[this->size() - 1]; }     //取顶，获取栈顶数据

  //复习：数组引用，数组指针和多维数组
};

#endif