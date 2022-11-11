#ifndef BINNODE_H
#define BINNODE_H

enum RBColor { RB_RED,
               RB_BLACK }; //红黑树节点颜色

template <typename T> class BinNode; //预先声明类
template <typename T>
using BinNodePosi = BinNode<T> *; //节点位置

template <typename T>
class BinNode { //二叉树节点模板类

 private:
  //成员
   BinNodePosi<T> pt;     //父节点
   BinNodePosi<T> lc, rc; //左、右节点
   T elem;                //数值
   int ht;                //高度(通用)
   int npl;               // Null Path Length(左氏堆，也可以用height直接替代)
   RBColor color;         //颜色(红黑树)

 public:
  //构造函数
   BinNode() : 
    pt(nullptr), lc(nullptr), rc(nullptr), ht(0), npl(1), color(RB_RED) {}                                                                                                                         //默认构造函数
   BinNode(T e, BinNodePosi<T> p = nullptr, BinNodePosi<T> l = nullptr, BinNodePosi<T> r = nullptr, int h = 0; int npl = 1; RBColor c = RB_RED) : 
    data(e), pnt(p), lc(l), rc(r), ht(h), npl(npl), color(c) {} //带参data构造函数

   //操作接口
   int size();                            //统计当前节点后代数，即其根子树规模
   BinNodePosi<T> insertAsLC(T const &e); //将元素e当作当前节点的左孩子插入为新节点
   BinNodePosi<T> insertAsRC(T const &e); //将元素e当作当前节点的右孩子插入为新节点
   BinNodePosi<T> succ();                 //（中序遍历意义下）当前节点的直接后继节点

   T &data() { return elem; }              //获得当前节点所存数据对象————可修改的左值
   BinNodePosi<T> &parent() { return pt; } //获得当前节点的父节点位置
   BinNodePosi<T> &lChild() { return lc; }   //获得当前节点的左子节点位置
   BinNodePosi<T> &rChild() { return rc; }   //获得当前节点的右子节点位置
   int &height() { return ht; }            //获得当前节点高度
   int &getNpl() { return npl; }           //获得当前节点左氏堆值
   RBColor &getColor() { return color; }   //获得当前节点的颜色

   template <typename VST> void travLevel(VST &); //子树层次遍历
   template <typename VST> void travPre(VST &);   //子树先序遍历
   template <typename VST> void travIn(VST &);    //子树中序遍历
   template <typename VST> void travPost(VST &);  //子树后序遍历

   //比较器、判等器（操作符重载）
   bool operator< (BinNode const &bn) {return elem < bn.data();}   //重载小于号
   bool operator> (BinNode const &bn) {return elem > bn.data();}   //重载大于号
   bool operator== (BinNode const &bn) {return elem == bn.data();} //判等重载
   bool operator!= (BinNode const &bn) {return elem != bn.data();} //不等重载
   bool operator>= (BinNode const &bn) {return elem >= bn.data();} //重载大于等于号
   bool operator<= (BinNode const &bn) {return elem <= bn.data();} //重载小于等于号
};
/********************************函数实现************************************/


#endif