#ifndef LISTNODE_H
#define LISTNODE_H

using Rank = int; //秩

template <typename T> class ListNode;
template <typename T> using ListNodePosi = ListNode<T>*; //列表节点位置

template <typename T> 
class ListNode { //列表节点模板类
 private:
  T data; //数值
  ListNodePosi<T> pre; //前驱节点
  ListNodePosi<T> suc; //后继节点

 public:
//构造函数
  ListNode {}; //默认构造函数，针对头节点header和尾节点trailer构造
  ListNode (T e, ListNodePosi<T> p = NULL, ListNodePosi<T> s = NULL)
        : data(e), pre(p), suc(s) {}; //默认构造器
  
//操作接口
  ListNodePosi<T> pred() { return this->pre; } //获得前驱节点位置
  ListNodePosi<T> succ() { return this->suc; }
  T data() { return this->data; } //获得当前节点索寸数据对象

  ListNodePosi<T> insertAsPred(T const& e); //插入前驱节点，返回其位置
  ListNodePosi<T> insertAsSucc(T const& e);
};

template <typename T> 
ListNodePosi<T> ListNode<T>::insertAsPred(T const& e) {

}






#endif