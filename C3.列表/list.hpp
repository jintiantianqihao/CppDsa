#ifndef LIST_H
#define LIST_H

#include <iostream>
#include "listnode.hpp" //引入列表节点类
using std::cout;
using std::endl;

template <typename T>
class List { //列表模板类
 private:
   int _size;              //规模
   ListNodePosi<T> header; //头哨兵
   ListNodePosi<T> tailer; //尾哨兵（哨兵均不在列表的可见数据壳内）

 protected:
   void init();                          //列表创建时的初始化，需要统一调用
   int clear();                          //清除所有节点
   void copyNodes(ListNodePosi<T>, int); //复制列表中自位置p起的n项
   ListNodePosi<T> merge(ListNodePosi<T>, int, List<T> &, ListNodePosi<T>, int); //归并
   void mergeSort(ListNodePosi<T> &, int);   //对从p开始连续的n个节点归并排序
   void selectionSort(ListNodePosi<T>, int); //对从p开始连续的n个节点选择排序
   void insertionSort(ListNodePosi<T>, int); //对从p开始连续的n个节点插入排序
   void radixSort(ListNodePosi<T>, int);     //对从p开始连续的n个节点基数排序

 public:
   // 构造函数
   List() { init(); }                                         //默认
   List(const List<T> &L) { copyNodes(L.first(), L.size()); } //整体复制列表L
   List(const List<T> &L, Rank r, int n)                      //复制列表L中自第r项起的n项
   { ListNodePosi<T> p = L.first(); Rank k = 0; while (k++ < r) p = p->succ(); copyNodes(p, n); } //header最好不要在外暴露，仅在特殊情况下使用，考虑清楚边界情况啊                         
   List(ListNodePosi<T> p, int n) { copyNodes(p, n); }        //复制列表中自位置p起的n项

   // 析构函数
   ~List() { clear(); delete header; delete tailer; } //释放（包含头、尾哨兵在内的）所有节点

   // 只读访问接口
   Rank size() const { return _size; }                                           //规模
   bool empty() const { return _size <= 0; }                                     //判空
   T &operator[](Rank r) const;                                                  //重载，支持循秩访问（效率低）
   ListNodePosi<T> first() const { return header->succ(); }                      //首节点位置
   ListNodePosi<T> last() const { return tailer->pred(); }                       //末节点位置
   bool valid(ListNodePosi<T> p)                                                 //判断位置p是否对外合法
   { return p && (tailer != p) && (header != p); }                               //将头、尾节点等同于nullptr
   ListNodePosi<T> find(const T &e) const { return find(e, _size, tailer); }     //无序列表查找
   ListNodePosi<T> find(const T &e, int n, ListNodePosi<T> p) const;             //无序区间向前查找
   ListNodePosi<T> find(const T &e, ListNodePosi<T> p, int n) const;             //无序区间向后查找
   ListNodePosi<T> search(const T &e) const { return search(e, _size, tailer); } //有序列表查找
   ListNodePosi<T> search(const T &e, int n, ListNodePosi<T> p) const;           //有序区间向前查找
   ListNodePosi<T> search(const T &e, ListNodePosi<T> p, int n) const;           //有序区间向后查找
   ListNodePosi<T> selectMax(ListNodePosi<T> p, int n) const;                          //在p及其n-1个后继中选出最大者
   ListNodePosi<T> selectMax() const { return selectMax(header->succ(), _size); }      //整体最大者

   // 可写访问接口
   ListNodePosi<T> insertAsFirst(const T &e);                   //将e当作首节点插入
   ListNodePosi<T> insertAsLast(const T &e);                    //将e当作末节点插入
   ListNodePosi<T> insertAfter(ListNodePosi<T> p, const T &e);  //将e当作p的后继插入
   ListNodePosi<T> insertBefore(const T &e, ListNodePosi<T> p); //将e当作p的前驱插入
   T remove(ListNodePosi<T> p);                                 //删除合法位置p处的节点,返回被删除节点
   void merge(List<T> &L) { merge(header->succ(), _size, L, L.header->succ(), L._size); } //全列表归并
   void sort(ListNodePosi<T> p, int n);  //列表区间排序
   void sort() { sort(first(), _size); } //列表整体排序
   int deduplicate();                    //无序去重
   int uniquify();                       //有序去重
   void reverse();                       //前后倒置（习题）

   // 遍历
   void traverse(void (*)(T &)); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
   template <typename VST>       //操作器
   void traverse(VST);           //遍历，依次实施visit操作（函数对象，可全局性修改,不能使用引用形式，因为会面临temp常量赋值现象
};// List

//辅助函数
template <typename T> void print(const ListNodePosi<T> &);
template <typename T> void mySwap(T& , T&);
//辅助类
template <typename T>
class myPrint {
public:
  virtual void operator()(T &e) {
    cout << e << " ";
  }
};


//****************************************************************实现部分*********************************************************************//


/*0.双向列表前后倒置算法*/
////数据域交换法 (光换头尾是没法实现的，因为内部每个节点的前后都要翻转)
template <typename T>
void List<T>::reverse() { //O(n)
  if (empty()) return;
  ListNodePosi<T> head = first(), tail = last();
  while ((head != tail) && (head->pred() != tail)) {
    mySwap(head->elem, tail->elem); //需要修改值，不可用常量，利用友元类
    head = head->succ();
    tail = tail->pred();
  }
}


/*1.重载下标操作符，循秩访问（低效，仅作为左值访问）*/
template <typename T>
T& List<T>::operator[](Rank r) const { // O(n)
  ListNodePosi<T> temp = first();
  for (Rank i = 0; i < r; ++i) {
    temp = temp->succ();
  }

  return temp->elem;
}

/*2.列表初始化*/
template <typename T>
void List<T>::init() { //初始化，创建列表对象时统一调用
  header = new ListNode<T>;
  tailer = new ListNode<T>; //创建头节点哨兵
  _size = 0;                //初始化规模

  header->suc = tailer;
  header->pre = nullptr; //互联
  tailer->pre = header;
  tailer->suc = nullptr; //封装写法header->succ() = tailer; header->pred() = nullptr;
}

/*3.插入函数*/
//向前插入
template <typename T>
ListNodePosi<T> List<T>::insertBefore(const T &e, ListNodePosi<T> p) { //O(1)
  ++_size;
  return p->insertAsPred(e); //链接已调整好，因为双向链表能向前回溯的缘故，因此不用利用快慢指针来实现了
}
//向后插入————————————插入的时候就已经扩充容量了，因此这里都是在基本操作上封装好的，不用在进行其他调用时扩容了，否则会冗余，导致容量不匹配*********************
template <typename T>
ListNodePosi<T> List<T>::insertAfter( ListNodePosi<T> p, const T &e) { //O(1)
  ++_size;
  return p->insertAsSucc(e); //链接已调整好，因为双向链表能向前回溯的缘故，因此不用利用快慢指针来实现了
}
//头插法
template <typename T>
ListNodePosi<T> List<T>::insertAsFirst(const T &e) { //O(1)
  return insertAfter(header, e);
}
//尾插法
template <typename T>
ListNodePosi<T> List<T>::insertAsLast(const T &e) { //O(1)
  return insertBefore(e, tailer);
}

//列表复制函数（构造调用）
// copy函数的抽象性＋封装性进一步体现
template <typename T>
void List<T>::copyNodes(ListNodePosi<T> p, int n) { // O(n)
  init();                                           //隐性传入了this指针

  while (_size < n) {      // while (_size++ < n) {
    insertAsLast(p->elem); // insertAsLast(p->elem); *****************插入操作已经扩容啦，条件里就别再乱来啦
    p = p->succ();         // p = p->succ();
  }                        //}
}

/*4.列表节点删除*/
//删除函数
template <typename T>
T List<T>::remove(ListNodePosi<T> p) { //O(1)
  T e = p->elem;
  p->suc->pre = p->pre;
  p->pre->suc = p->suc;

  delete p; //写入nullptr操作在规模较大时容易影响效率，一般在申请空间前做判断或赋初值即可
  --_size;  //规模缩减调整
  return e;
}

//列表节点清空（析构调用）
template <typename T>
int List<T>::clear() { // O(n)
  int oldSize = _size;
  while (_size > 0) {    //<=> while(_size > 0)
    remove(header->succ()); //与之前单向链表无_size的思路不同，并且此处的遍历不用主动迭代，链接会自动调整
  }
  return oldSize; //返回被清空的链表规模
}

/*5.无序列表查找*/
//前向查找
template <typename T>
ListNodePosi<T> List<T>::find(const T &e, int n, ListNodePosi<T> p) const { //O(n)
  while (n-- > 0) { 
    if (e == p->pred()->elem) { //找到
      return p->pred();
    } else
      p = p->pred(); //前向递增
  }
  return nullptr; //查找失败
}

//后向查找(若重复则返回秩最小者)
template <typename T>
ListNodePosi<T> List<T>::find(const T &e, ListNodePosi<T> p, int n) const { //O(n)
  while (n-- > 0) { 
    if (e == p->succ()->data) { //找到
      return p->succ();
    } else
      p = p->succ(); //后向递增
  }
  return nullptr; //查找失败
}

/*6.无序列表去重（调用前向查找，采用分治思想）*/
template <typename T>
int List<T>::deduplicate() { // O(n^2)
  int oldSize = _size;
  Rank r = 0;
  for (ListNodePosi<T> p = first(); p != tailer; p = p->succ()) { // O(n)
    ListNodePosi<T> q = find(p->elem, r, p);                    // O(n)
    q ? remove(q) : ++r;
  }
  return oldSize - _size; //返回删除的重复元素数量
}

/*7.链表遍历*/
template <typename T>
template <typename VST>
void List<T>::traverse(VST visit) {
  for (ListNodePosi<T> temp = first(); temp != tailer; temp = temp->succ())
    visit(temp->elem);
  return;
}

//函数对象实现遍历打印**(理解如何调用的)
template <typename T>
void print(List<T> &L) {
  L.traverse(myPrint<T>()); //相当于传入了一种操作
  cout << endl;
}

/**********************有序列表************************/
/*8.有序列表去重*/
template <typename T>
int List<T>::uniquify() { //O(n)
  if(_size < 2) return 0; //保证至少有两个节点，平凡情况直接略过
  Rank oldSize = _size;
  ListNodePosi<T> p = first();
  for(ListNodePosi<T> q = p->succ(); q != tailer; q = p->succ()) {
    if (p->elem == q->elem)
      remove(q);
    else
      p = p->succ();
  }

  return oldSize - _size;
}

/*9.有序列表查找*/
////可定义语义：返回不大于e的秩最大者
//向前查找-->查不含p的前n个
template <typename T>
ListNodePosi<T> List<T>::search(const T &e, int n, ListNodePosi<T> p) const { // O(n)
  p = p->pred();
  for (Rank i = 0; i < n; ++i) {
    if (p->elem <= e)
      break;
    p = p->pred();
  }
  return p;
}

//向后查找
template <typename T>
ListNodePosi<T> List<T>::search(const T &e, ListNodePosi<T> p, int n) const { // O(n)
  p = p->pred();
  for (Rank i = 0; i < n; ++i) { // for (Rank i = 0; i < n; ++i) {
    if (p->elem <= e)          //   p = p->pred();
      break;                     //    if(p->data <= e)  ////要考虑边界情况呀，细节错误，警钟长鸣！
    p = p->pred();               //       break;
  }                              //  }
  return p;                      //***没有考虑边界情况，未充分发挥break作用
}

//************************************排序算法**********************************************//
//统一接口
template <typename T>
void List<T>::sort(ListNodePosi<T> p, int n) {
  switch (2) {
    case 0:                
      selectionSort(p, n); //选择排序
      break;               
    case 1:                
      insertionSort(p, n); //插入排序
      break;               
    case 2:                
      mergeSort(p, n);     //归并排序
      break;               
    default:               
      radixSort(p, n);     //基数排序
      break;
  }
}

/*10.选择排序算法（待优化）*///交换是核心
//从p往前排n个————p+n
//交换
template <typename T>
void mySwap(T &a, T &b) {
  T temp = a;
  a = b;
  b = temp;
}

//求最大值————油画算法（含p的前n个数）
template <typename T>
ListNodePosi<T> List<T>::selectMax(ListNodePosi<T> p, int n) const {
  ListNodePosi<T> maxPosi = p;
  while (--n) { //排n-1次
    p = p->pred();
    if (maxPosi->elem <= p->elem)
      maxPosi = p;
  }
  return maxPosi;
}

/*
//高new、delete消耗法
template <typename T>
void List<T>::selectionSort(ListNodePosi<T> p, int n) {
  ListNodePosi<T> head = p->pred(), tail = p; //确定排序区间(head,tail) = [p,p+n)--->统一头尾哨兵语义
  for (int i = 0; i < n; ++i)
    tail = tail->succ(); //找到尾部界桩

  while (n > 1) { //排n-1次就行啦
    insertBefore(remove(selectMax(tail->pred(), n)), tail); //找到最大值删除，将返回值插在tail前：O(100) //本质是以插入实现交换
    tail = tail->pred();
    --n;
  }
}
*/

//修改tail和max局部引用优化

//只交换数据域，小优化
template <typename T>
void List<T>::selectionSort(ListNodePosi<T> p, int n) {
  ListNodePosi<T> head = p->pred(), tail = p; //确定排序区间(head,tail) = [p,p+n)--->统一头尾哨兵语义
  for (int i = 0; i < n; ++i)
    tail = tail->succ(); //找到尾部界桩

  while (n > 1) { //排n-1次就行啦
    mySwap( selectMax(tail->pred(), n)->elem, tail->pred()->elem ); //和tail前的值交换
    tail = tail->pred();
    --n;
  }
}

/*11.插入排序算法*///插入是核心操作
//对从p开始连续的n个节点插入排序（定位+插入---含p）
//低效new、delete版
template <typename T>
void List<T>::insertionSort(ListNodePosi<T> p, int n) { //O(n^2)
  p = p->succ();           //确定排序区间 [p,p+n),但此处第一个无需排好,这里的p即为tail
  for (int r = 1; r < n; ++r) {
    insertAfter( search(p->elem, r, p), p->elem ); //稳定性保障了：O(i)~~O(i+100)
    p = p->succ();
    remove(p->pred()); // O(1)~~实际上O(100)
  }                    // 就地算法，辅助空间复杂度O(1);
}

/*12.归并排序算法*///分治+二路归并是核心
//主体算法
//对从p开始连续的n个节点归并排序(含p)
template <typename T>
void List<T>::mergeSort(ListNodePosi<T> &p, int n) { //O(nlogn),但是极度占栈空间，相当于以空间换时间了,利用主定理
  if (n < 2) return; //递归基

  int m = n / 2;
  ListNodePosi<T> q = p;
  for (int i = 0; i < m; ++i)  q = q->succ(); //找到分治中点 O(m)=O(n)

  mergeSort(p, m);
  mergeSort(q, n - m);              //分治
  p = merge(p, m, *this, q, n - m); //二路归并：O(r)
  return;
}

//二路归并
template <typename T>
ListNodePosi<T> List<T>::merge(ListNodePosi<T> p, int n, List<T> &L, ListNodePosi<T> q, int m) { //O(n)
  ListNodePosi<T> phead = p->pred();
  while ((n > 0) && (m > 0)) {
    if (q->elem < p->elem) {
      insertBefore(q->elem, p); //考虑此处排序稳定性能保障
      q = q->succ();
      L.remove(q->pred());
      --m; //对应好谁是谁的计数下标！！！！！！不要张冠李戴！！！！！（全是细节）
    } else {
      p = p->succ();
      --n;
    }
  }
  return phead->succ();
}

#endif
