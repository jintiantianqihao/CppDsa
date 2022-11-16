#ifndef BINTREE_HPP
#define BINTREE_HPP

#include <iostream>
#include "binnode.hpp"
using std::cin;
using std::cout;
using std::endl;

//辅助函数
int max(int const &a, int const &b) {
  return (a > b) ? a : b;
}

template <typename T>
class BinTree { //二叉树类

 protected:

  //要素成员
  int _size;            //规模
  BinNodePosi<T> _root; //根节点

  //定义空树高度（只在类内调用）————用内联函数替换宏定义，在编译时就进行替换，效率和宏函数相当
  inline int stature(BinNodePosi<T> p) { (p == nullptr) ? -1 : p->height(); } //此处也可以不用inline，类内直接内联

  //树内部迭代
  virtual int updateHeight(BinNodePosi<T> x); //更新节点x的高度（虚方法便于不同种类树继承高度情况，类外定义时不必再加虚函数类型）
  void updataHeightAbove(BinNodePosi<T> x);   //更新节点x及其祖先高度

 public:
  
  //构造析构函数
  BinTree(): _size(0), root(nullptr) {}    //构造函数
  ~BinTree() { if(_size > 0) remove(root); } //析构函数

  //操作函数
  int size() const { return _size; }      //规模获取————函数后const表示函数不可修改类内成员
  bool empty() const { return !_root; }   //判空
  BinNodePosi<T> root() { return _root; } //树根获取
};

//***************************************实现部分*************************************************//

//1.节点x高度更新函数
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi<T> x) { //O(1):普通二叉树的高度定义下
  x->height() = max(stature(x->lChild()), stature(x->rChild())) + 1;
  return x->height();
}

//2.节点x及其祖先高度依次更新(全树高度更新)
template <typename T>
void BinTree<T>::updataHeightAbove(BinNodePosi<T> x) {
  while (x != root()) {
    updataHeight(x);
    x = x->parent();
  }
  updateHeight(root());

  return;
}

#endif