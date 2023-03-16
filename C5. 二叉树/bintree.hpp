#ifndef BINTREE_HPP
#define BINTREE_HPP

#include <iostream>
#include "binnode.hpp"
#include "release.h"
using std::cin;
using std::cout;
using std::endl;




//辅助函数
int max(const int &a, const int &b) { //const在引用左侧修饰的是&底层const，但本身可变；const在引用右侧修饰的是a，引用本身不可变但指向的值可变
  return (a > b) ? a : b;
}





template <typename T>
class BinTree { //二叉树类

 protected:

  ////要素成员
  int _size;            //规模
  BinNodePosi<T> _root; //根节点（一个标记指针，不产生冗余的堆内存

  //friend BinNodePosi<T> &FromParentTo(BinNodePosi<T> x); //获得x在树中相应位置的引用

  static int stature(BinNodePosi<T> p) { return (p == nullptr) ? -1 : p->height(); } //////***********定义空树高度（只在类内调用）:此处也可以不用inline，类内直接内联————用内联函数替换宏定义，在编译时就进行替换，效率和宏函数相当
  virtual int updateHeight(BinNodePosi<T> x); //更新节点x的高度（虚方法便于不同种类树继承高度情况，类外定义时不必再加虚函数类型）
  void updateHeightAbove(BinNodePosi<T> x);   //更新节点x及其祖先高度

public:
  
  ////构造析构
  BinTree(): _size(0), _root(nullptr) {}     //构造函数
  //~BinTree() { if(_size > 0) remove(_root); } //析构函数

  ////操作函数
  int size() const { return _size; }                       //规模获取————函数后const表示函数不可修改类内成员
  bool empty() const { return !_root; }                    //判空
  BinNodePosi<T> root() const { return _root; }            //树根获取
  //可修改的树根获取

  BinNodePosi<T> insert(const T &e);                       //根节点插入：insertAsROOT
  BinNodePosi<T> insert(BinNodePosi<T> x, T const &e);     //右孩子插入: insertAsRC
  BinNodePosi<T> insert(T const &e, BinNodePosi<T> x);     //左孩子插入: insertAsLC
  BinNodePosi<T> attach(BinNodePosi<T> x, BinTree<T> *&S); //左树接入：insertLT
  BinNodePosi<T> attach(BinTree<T> *&S, BinNodePosi<T> x); //右树接入：insertRT
  int remove(BinNodePosi<T> x);                            //子树删除：删除以位置x处的节点为根的子树，并返回原子树规模
  BinTree<T> *secede(BinNodePosi<T> x);                    //子树分离：将x处子树分离，封装并返回一棵独立子树
};



//****************************************************************************************************************************
//****************************************************************************************************************************
//********************************************************* 实 ***************************************************************
//********************************************************* 现 ***************************************************************
//********************************************************* 部 ***************************************************************
//********************************************************* 分 ***************************************************************
//****************************************************************************************************************************
//****************************************************************************************************************************


////1.节点x高度更新函数
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi<T> x) { //O(1):普通二叉树的高度定义下
  x->ht = max(stature(x->lChild()), stature(x->rChild())) + 1;
  return x->height();
}

////2.节点x及其祖先高度依次更新(全树高度更新)
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi<T> x) { //O(n = height):最大高度遍历——————最深叶节点加子树啦
  while (x != root()) {
    updateHeight(x);
    x = x->parent();
  }
  updateHeight(root());

  return;
}

////3.节点插入函数
//insertAsRC
template <typename T>
BinNodePosi<T> BinTree<T>::insert(BinNodePosi<T> x, const T &e) {
  ++_size;              //插入前先判断，此处默认x右子树空，插入必成功
  x->insertAsRC(e);     //插入做右子树(在此处就完成了右子树更新，具体见binnode实现)
  updateHeightAbove(x); // x及其祖先节点高度可能变，其余节点必不变；

  return x->rChild();
}

//insertAsLC
template <typename T>
BinNodePosi<T> BinTree<T>::insert(const T &e, BinNodePosi<T> x) {
  ++_size;              //插入前先判断，此处默认x左子树空，插入必成功
  x->insertAsLC(e);     //插入做右子树(在此处就完成了右子树更新，具体见binnode实现)
  updateHeightAbove(x); //x及其祖先节点高度可能变，其余节点必不变；

  return x->lChild();
}

//insertAsROOT
template <typename T>
BinNodePosi<T> BinTree<T>::insert(const T &e) {
  _size = 1;                 //插入前先做判断，此处默认二叉树为空
  _root = new BinNode<T>(e); //利用构造函数进行初始化传参(申请内存Q1)

  return root();
}

////4.子树接入函数(内存防泄漏是关键)？？？？？？？？？？
//insertLT：1.调整链接 ——> 2.更新树的属性 ——> 3.释放节点
template <typename T>
BinNodePosi<T> BinTree<T>::attach(BinNodePosi<T> x, BinTree<T> *&S) {
  x->lc = S->_root; //x左子为接入的树根部
  S->_root->pt = x; //同上均为调整链接

  _size += S->size();   //节点数修正
  updateHeightAbove(x); //高度修正

  S->_root = nullptr;
  S->_size = 0; //消除通过S访问的模式
  S = nullptr;

  return x;
}

//insertRT
template <typename T>
BinNodePosi<T> BinTree<T>::attach(BinTree<T> *&S, BinNodePosi<T> x) {
  x->rc = S->_root; //x右子为接入的树根部
  S->_root->pt = x; //同上均为调整链接

  _size += S->size();   //节点数修正
  updateHeightAbove(x); //高度修正

  S->_root = nullptr;
  S->_size = 0; //消除通过S访问的模式
  S = nullptr;  //在栈上而非堆上，只用重新置空即可摧毁，不必release

  return x;
}


#endif