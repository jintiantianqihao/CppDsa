#include <iostream>
using std::cin;
using std::cout;
using std::endl;

enum RBColor { RB_RED,
               RB_BLACK }; //红黑树节点颜色

template <typename T> class BinNode; //预先声明类
template <typename T>
using BinNodePosi = BinNode<T> *; //节点位置

//辅助函数
int max(const int &a, const int &b) { //const在引用左侧修饰的是&底层const，但本身可变；const在引用右侧修饰的是a，引用本身不可变但指向的值可变
  return (a > b) ? a : b;
}

//*****************************************************************************************//
//********************************BinNode节点类实现*****************************************//
//*****************************************************************************************//
template <typename T>
class BinNode { //二叉树节点模板类

 private:
  //成员
   BinNodePosi<T> pt;     //父节点
   BinNodePosi<T> lc, rc; //左、右节点
   T elem;                //数值
   int ht;                //高度(通用)
   int npl;               //Null Path Length(左氏堆，也可以用height直接替代)
   RBColor color;         //颜色(红黑树)

 public:
  //构造函数
   BinNode() : 
    pt(nullptr), lc(nullptr), rc(nullptr), ht(0), npl(1), color(RB_RED) {}                                                                                                                         //默认构造函数
   BinNode(T e, BinNodePosi<T> p = nullptr, BinNodePosi<T> l = nullptr, BinNodePosi<T> r = nullptr, int h = 0, int npl = 1, RBColor c = RB_RED) : 
    data(e), pnt(p), lc(l), rc(r), ht(h), npl(npl), color(c) {} //带参data构造函数

   //操作接口
   int size();                            //统计当前节点后代数，即其根子树规模
   BinNodePosi<T> insertAsLC(T const &e); //将元素e当作当前节点的左孩子插入为新节点
   BinNodePosi<T> insertAsRC(T const &e); //将元素e当作当前节点的右孩子插入为新节点
   BinNodePosi<T> succ();                 //（中序遍历意义下）当前节点的直接后继节点

   T &data() { return elem; }              //获得当前节点所存数据对象————可修改的左值
   BinNodePosi<T> &parent() { return pt; } //获得当前节点的父节点位置
   BinNodePosi<T> &lChild() { return lc; } //获得当前节点的左子节点位置
   BinNodePosi<T> &rChild() { return rc; } //获得当前节点的右子节点位置
   int &height() { return ht; }            //获得当前节点高度
   int &getNpl() { return npl; }           //获得当前节点左氏堆值
   RBColor &getColor() { return color; }   //获得当前节点的颜色

   template <typename VST> void travLevel(VST &); //子树层次遍历
   template <typename VST> void travPre(VST &);   //子树先序遍历
   template <typename VST> void travIn(VST &);    //子树中序遍历
   template <typename VST> void travPost(VST &);  //子树后序遍历

   //比较器、判等器（操作符重载）
   bool operator< (BinNode const &bn) {return elem < bn.elem;}   //重载小于号
   bool operator> (BinNode const &bn) {return elem > bn.elem;}   //重载大于号
   bool operator== (BinNode const &bn) {return elem == bn.elem;} //判等重载
   bool operator!= (BinNode const &bn) {return elem != bn.elem;} //不等重载
   bool operator>= (BinNode const &bn) {return elem >= bn.elem;} //重载大于等于号
   bool operator<= (BinNode const &bn) {return elem <= bn.elem;} //重载小于等于号
};
/********************************函数实现************************************/

template <typename T>
BinNodePosi<T> BinNode<T>::insertAsLC(T const &e) {
  return lc = new BinNode(e, this); //调用构造函数
} //O(1)

template <typename T>
BinNodePosi<T> BinNode<T>::insertAsRC(T const &e) {
  return rc = new BinNode(e, this); //调用构造函数
} //O(1)

//递归实现后代节点总数求解
template <typename T> int BinNode<T>:: size() { //O(n) = O(|size|)
  int s = 1; //计算自身
  if (lc != nullptr)  return s += lc->size(); //递归计入左子树规模
  if (rc != nullptr)  return s += rc->size(); //递归计入右子树规模

  return s;
}


//*****************************************************************************************//
//***********************************BinTree类实现*****************************************//
//*****************************************************************************************//
template <typename T>
class BinTree { //二叉树类

 protected:

  //要素成员
  int _size;            //规模
  BinNodePosi<T> _root; //根节点

  //***********定义空树高度（只在类内调用）————用内联函数替换宏定义，在编译时就进行替换，效率和宏函数相当
  inline int stature(BinNodePosi<T> p) { (p == nullptr) ? -1 : p->height(); } //此处也可以不用inline，类内直接内联

  //树内部迭代
  virtual int updateHeight(BinNodePosi<T> x); //更新节点x的高度（虚方法便于不同种类树继承高度情况，类外定义时不必再加虚函数类型）
  void updataHeightAbove(BinNodePosi<T> x);   //更新节点x及其祖先高度

 public:
  
  //构造析构函数
  BinTree(): _size(0), root(nullptr) {}    //构造函数
  ~BinTree() { if(_size > 0) remove(root); } //析构函数

  //操作函数
  int size() const { return _size; }            //规模获取————函数后const表示函数不可修改类内成员
  bool empty() const { return !_root; }         //判空
  BinNodePosi<T> root() const { return _root; } //树根获取

  BinNodePosi<T> insert(BinNodePosi<T> x, T const &e); //右孩子插入:insertAsRC
  BinNodePosi<T> insert(T const &e, BinNodePosi<T> x); //左孩子插入:insertAsLC
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
void BinTree<T>::updataHeightAbove(BinNodePosi<T> x) { //O(n = height):最大高度遍历——————最深叶节点加子树啦
  while (x != root()) {
    updataHeight(x);
    x = x->parent();
  }
  updateHeight(root());

  return;
}

////3.节点插入函数
//insertAsRC
template <typename T>
BinNodePosi<T> BinTree<T>::insert(BinNodePosi<T> x, T const &e) {
  ++_size;              //插入前先判断，此处默认x右子树空，插入必成功
  x->insertAsRC(e);     //插入做右子树(在此处就完成了右子树更新，具体见binnode实现)
  updataHeightAbove(x); // x及其祖先节点高度可能变，其余节点必不变；

  return x->rChild();
}

//insertAsLC
template <typename T>
BinNodePosi<T> BinTree<T>::insert(T const &e, BinNodePosi<T> x) {
  ++_size;              //插入前先判断，此处默认x左子树空，插入必成功
  x->insertAsLC(e);     //插入做右子树(在此处就完成了右子树更新，具体见binnode实现)
  updataHeightAbove(x); // x及其祖先节点高度可能变，其余节点必不变；

  return x->lChild();
}


int main(void)
{
  BinNode<int> skt;
}
