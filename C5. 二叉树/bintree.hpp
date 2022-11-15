#ifndef BINTREE_HPP
#define BINTREE_HPP

#include <iostream>
#include "binnode.hpp"
using std::cin;
using std::cout;
using std::endl;

template <typename T>
class BinTree { //二叉树类

 protected:

  //要素成员
  int _size;           //规模
  BinNodePosi<T> _root; //根节点

  //树内部迭代
  virtual int updateHeight(BinNodePosi<T> x); //更新节点x的高度（虚方法便于不同种类树继承高度情况）
  void updataHeightAbove(BinNodePosi<T> x);   //更新节点x及其祖先高度

 public:
  
  //构造析构函数
  BinTree(): _size(0), root(nullptr) {}    //构造函数
  ~BinTree() {if(_size > 0) remove(root);} //析构函数

  //操作函数
  int size() const { return _size; }      //规模获取————函数后const表示函数不可修改类内成员
  bool empty() const { return !_root; }   //判空
  BinNodePosi<T> root() { return _root; } //树根获取
};

#endif