#ifndef QUEUE_H
#define QUEUE_H

#include "list.hpp" //以列表为基类，派生出队列模板类

template <typename T>
class Queue : public List<T> { //均为O(1)
public:
  void enqueue(T const &e) { this->insertAsLast(e); } //入队，列表尾插法
  T dequeue() { return this->remove(this->first()); }       //出队，队首删除，即列表头删除
  T &front() { return this->first()->data(); }        //队首查询
  T &rear() { return this->last()->data(); }          //队尾查询(单链表O(n))
};

#endif