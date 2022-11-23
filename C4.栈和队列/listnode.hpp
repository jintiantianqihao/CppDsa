#ifndef LISTNODE_H
#define LISTNODE_H

//调用多个头文件时，注意作用域啊；另外，尽量使用对外的函数接口访问操作类内私有变量，避免出错
using Rank = int; //秩

template <typename T> class ListNode;
template <typename T> using ListNodePosi = ListNode<T> *; //列表节点位置

//list类事先声明，便于后面声明友元类
template <typename T> class List;

template <typename T>
class ListNode { //列表节点模板类
 private:
   T elem;                        //数值
   ListNodePosi<T> pre = nullptr; //前驱节点
   ListNodePosi<T> suc = nullptr; //后继节点
    
   friend class List<T>;          //声明友元类，便于接口封装

 public:
   //构造函数
   ListNode() {}; //默认构造函数，针对头节点header和尾节点trailer构造
   ListNode(T e, ListNodePosi<T> p = nullptr, ListNodePosi<T> s = nullptr) //////////////?????????模板类型如何定义默认初始化呀
       : elem(e), pre(p), suc(s){}; //默认构造器

   //操作接口
   const ListNodePosi<T> pred() const { return pre; } //获得前驱节点位置
   const ListNodePosi<T> succ() const { return suc; } //获得后继节点位置
   T data() const { return elem; }                    //获得当前节点所存数据对象

   ListNodePosi<T> insertAsPred(T const &e); //插入前驱节点，返回其位置
   ListNodePosi<T> insertAsSucc(T const &e); //插入后继节点，返回其位置
};

//实现部分
template <typename T>
ListNodePosi<T> ListNode<T>::insertAsPred(T const &e) { // O(1)
  ListNodePosi<T> temp = new ListNode<T>(e, pre, this); //开辟堆空间，设置新节点
  pre->suc = temp;
  pre = temp; //修正链接

  return temp;
}

template <typename T>
ListNodePosi<T> ListNode<T>::insertAsSucc(T const &e) { // O(1)
  ListNodePosi<T> temp = new ListNode<T>(e, this, suc); //该步消耗较大
  suc->pre = temp;
  suc = temp; // this->suc = temp;

  return temp;
}
/*节点插入流程：1.申请堆空间 2.调整链接*/
#endif