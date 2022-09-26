#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>

using Rank = int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）
using std::cout,std::cin,std::endl;

template <typename T> 
class Vector { //向量模板类,循秩访问

 protected:
   /*三个成员对象*/
   Rank _size;     //规模(逻辑存储空间)
   Rank _capacity; //容量(物理存储空间)
   T *_elem;       //数据区

   void copyFrom(T const *A, Rank low, Rank high); //复制数组区间A[low, high)
   void expand();                                  //空间不足时扩容
   void shrink();                                  //装填因子过小时压缩
   bool bubble(Rank low, Rank high);               //扫描交换
   void bubbleSort(Rank low, Rank high);           //起泡排序算法
   Rank maxItem(Rank low, Rank high);              //选取最大元素
   void selectionSort(Rank low, Rank high);        //选择排序算法
   void merge(Rank low, Rank mi, Rank high);       //归并算法
   void mergeSort(Rank low, Rank high);            //归并排序算法
   void heapSort(Rank low, Rank high);             //堆排序（稍后结合完全堆讲解）
   Rank partition(Rank low, Rank high);            //轴点构造算法
   void quickSort(Rank low, Rank high);            //快速排序算法
   void shellSort(Rank low, Rank high);            //希尔排序算法

   //排序方法实现

 public:
   // 构造函数
   Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
   { _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v); }  //s<=c                                                                                // s<=c
    Vector(T const *A, Rank n) { copyFrom(A, 0, n); }                                 //数组整体复制
    Vector(T const *A, Rank low, Rank high) { copyFrom(A, low, high); }               //区间
    Vector(Vector<T> const &V) { copyFrom(V._elem, 0, V._size); }                     //向量整体复制
    Vector(Vector<T> const &V, Rank low, Rank high) { copyFrom(V._elem, low, high); } //区间

    // 析构函数
    ~Vector() { delete[] _elem; _elem = nullptr;}                                        //释放内部空间,并防止野指针

// 只读访问接口
    Rank size() const { return _size; }                                               //规模
    bool empty() const { return !_size; }                                             //判空
    Rank find(T const &e) const { return find(e, 0, _size); }                         //无序向量整体查找
    Rank find(T const &e, Rank low, Rank high) const;                                 //无序向量区间查找
    Rank search(T const &e) const { return (0 >= _size) ? -1 : search(e, 0, _size); } //有序向量整体查找
    Rank search(T const &e, Rank low, Rank high) const;                               //有序向量区间查找
    Rank disordered() const;                                                          //逆序对数计算

// 可写访问接口
    T &operator[](Rank r);                               //重载下标操作符，可以类似于数组形式引用各元素
    const T &operator[](Rank r) const;                   //仅限于做右值的重载版本
    Vector<T> &operator=(Vector<T> const &);             //重载赋值操作符，以便直接克隆向量
    T remove(Rank r);                                    //删除秩为r的元素
    int remove(Rank low, Rank high);                     //删除秩在区间[low, high)之内的元素
    Rank insert(Rank r, T const &e);                     //插入元素
    Rank insert(T const &e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank low, Rank high);                      //对[low, high)排序
    void sort() { sort(0, _size); }                      //整体排序
    void unsort(Rank low, Rank high);                    //对[low, high)置乱
    void unsort() { unsort(0, _size); }                  //整体置乱
    Rank deduplicate();                                  //无序去重
    Rank uniquify();                                     //有序去重

    // 遍历
    void traverse(void (*)(T &)); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST>
    void traverse(VST); //遍历（使用函数对象，可全局性修改）临时变量是常量引用，无法使用引用初始化
}; //Vector

// 辅助类
template <typename T>
class myPrint {
 public:
  virtual void operator()(T &e) { std::cout << e << " "; }
}; //类外类函数对象，单个打印：通过重载操作符()实现

template <typename T> 
class CheckOrder { //函数对象：判断一个T类对象是否局部有序
 public:
  T pre; 
  Rank &u; //此处声明了类型成员，定义需要在构造函数生成的过程中进行，并且这里必须有自定义构造函数，而不能依赖生成构造函数

  CheckOrder(Rank &unsorted, T &first) 
            : u(unsorted), pre(first) {} //类构造函数，初始化引用型对象u;
  virtual void operator()(T& e) { if (pre > e) ++u; pre = e;} //重载函数：找寻逆序对
};

class Fib { //Fib计算类
 private:
  Rank f, g;

 public:
  Fib(Rank n) { //初始化为不小于n的Fibonacci项
    f = 1, g = 0; //fib(0),fib(-1)
    while (g < n) { //O(log_phi(n))
      next();
    }
  }
  Rank get() { return g; } //获得当前项Fibonacci项 O(1)
  Rank next() { f = f + g; g = f - g; return g;} //转向下一项Fibonacci项 O(1)
  Rank prev() { g = f - g; f = f - g; return g;} //转向前一项Fibonacci项 O(1)
};

// 类外辅助函数声明(此时接口裸露，封装不好)
inline Rank myMax(Rank a, Rank b) { return (a > b) ? a : b; }                          //内联一个比较函数
template <typename T> inline void mySwap(T& a, T& b) { T temp = a; a = b; b = temp;}   //内联一个交换函数
template <typename T> void print(Vector<T> &v);                                        //向量遍历打印
template <typename T> void checkOrder(Vector<T> &v);                                   //向量有序性判定
/*二者同架构原理*/
template <typename T> static Rank binSearch(T* S, T const& e, Rank low, Rank high);    //二分查找A
template <typename T> static Rank fibSearch(T* S, T const& e, Rank low, Rank high);    //Fibonacci查找
/*二者实现了减少转向分支优化*/
template <typename T> static Rank binSearchB(T* S, T const& e, Rank low, Rank high);   //二分查找B
template <typename T> static Rank binSearchC(T* S, T const& e, Rank low, Rank high);   //二分查找C————语义，性能，功能最强优化
template <typename T> static Rank binSearchD(T* S, T const& e, Rank low, Rank high);   //二分查找D————C的递归版
/*用于实现大规模快速收敛*/
template <typename T> static Rank insertSearch(T* S, T const& e, Rank low, Rank high); //插值查找————小规模局部有病态


/***********************************************************实现部分(模板类不支持分离式编译)*************************************************************/


/*0.返回向量中最大值*/
template <typename T> 
Rank Vector<T>::maxItem(Rank low, Rank high) {
  Rank maxCnt  = 0;
  for (Rank i = low; i < high; ++i) {
    if (_elem[i] > _elem[maxCnt])
      maxCnt = i;
  }

  return maxCnt;
}

/*1.复制函数*/
//函数形式
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank low, Rank high) { // T为基本类型或者重载后的操作符
  _elem = new T[_capacity = 2 * (high - low)];              //分配空间，并为_capacity赋值
  _size = 0;                                                //大小初始化，规模清零
  while (low < high)
    _elem[_size++] = A[low++]; //复制到_elem[0,high - low)

  return;
}

//重载操作符赋值
template <typename T>
Vector<T>& Vector<T>::operator= (Vector<T> const& v) {
  if (_elem) {
    delete[] _elem; //释放内存(告诉计算机别人也可以用这段内存了)
    _elem = NULL;   //野指针置NULL
  }                 //指this指针所指向的当前对象，释放原有内容，
  copyFrom(v._elem, 0, v._size);
  return *this; //返回向量对象的引用，便于链式赋值
}

/*2.扩容函数*/
template <typename T>
void Vector<T>::expand() {                        //向量空间不足时扩容
  if (_size < _capacity) return;                  //尚未满员时，不必扩容
  _capacity = myMax(_capacity, DEFAULT_CAPACITY); //往大处扩容

  T *oldElem = _elem;
  _elem = new T[_capacity / 2]; //移位法扩大一倍
  for (Rank i = 0; i < _size; ++i) {
    _elem[i] = oldElem[i];
  }
  delete[] oldElem; //*释放原有空间
  return;
} //得益于向量封装，尽管扩容后数据区的物理地址有所改变，*但不会出现"野指针"

/*3.下标运算符重载：为了兼容原有运算符————循秩访问*/
template <typename T>
T &Vector<T>::operator[](Rank r) { //重载下标运算符
  return _elem[r];                 // 0 <= r <= _size
}

template <typename T>
const T &Vector<T>::operator[](Rank r) const { //只读仅仅做右值的重载下标符
  return _elem[r];                             // 0 <= r <= _size
}

/*4.插入函数*/
template <typename T>
Rank Vector<T>::insert(Rank r, T const &e) { // 0<=r<=_size
  expand();                                  //如有必要则扩容

  for (Rank i = _size; i > r; --i) { // O(n - r)：从后往前
    _elem[i] = _elem[i - 1];
  }
  _elem[r] = e; //插入新元素

  ++_size;  //容量更新
  return r; //返回秩
}

/*5.删除函数*/
//缩容函数
template <typename T>
void Vector<T>::shrink() {                      //装填因子过小时压缩空间，提高空间利用效率
  if (_capacity < DEFAULT_CAPACITY / 2) return; //保证不收缩到默认容量之下
  if (_size << 2 > _capacity) return;           //保证装填因子_size/_capacity在25%以下才进行缩容

  T *oldElem = _elem;                //另开辟新的动态内存空间
  _elem = new T[_capacity / 2];      //容量减半
  for (Rank i = 0; i < _size; ++i) { // O(n)
    _elem[i] = oldElem[i];
  }
  delete[] oldElem; //释放原空间

  return;
}

//区间删除
template <typename T>
int Vector<T>::remove(Rank low, Rank high) { // O(n -high)
  if (low == high) return 0;                 //为了提升算法效率，单独处理特殊简易的退化情况
  while (high < _size) {
    _elem[low++] = _elem[high++]; // O(high - low) : [high,_size)顺次前移，low更新到low + _size - high = n - (high - low)
  }
  _size = low; //更新规模
  shrink();    //如有必要则缩容,都是有this指针的

  return high - low; //返回被删除元素个数
}

//单元素删除：直接调用区间删除的特例版即可
template <typename T> 
T Vector<T>::remove(Rank r) { //O(n-r)
  T e = _elem[r]; //备份，用作返回值
  remove(r, r + 1);
  return e; //返回被删除的元素
}//若反之利用递归调用单元素删除实现区间删除，则会遭遇与循环移位相同的窘境：大量单次无效的前移，不能一步到位，从而导致时间复杂度为O(n^2)而非O(n)

/***********************无序向量********************/
/*6.查找函数*/
template <typename T> 
Rank Vector<T>::find(T const &e, Rank low, Rank high) const { //O(high - low), 0<=low<=high<=_size
  while ((low < high--) && (e != _elem[high]));//while ((e != _elem[--high]) && (low <= high)); (错误，可能在最后越下界访问到_elem[-1],发生段错误)
  return high; //返回值小于low时则查找失败；否则命中目标的秩，多值情况返回秩最大者  
}

/*7.无序去重函数*/
template <typename T> 
Rank Vector<T>::deduplicate() {
  Rank oldSize = _size;
  for (Rank i = 1; i < _size; ) {
    (find(_elem[i], 0, i) < 0) ? ++i : remove(i); //前缀钟寻找雷同者；没找到，继续找下一个；找到，删除该重复元素，并更新规模
  }
  
  return oldSize - _size; //返回所有重复元素的个数  
}

/*8.遍历函数————利用函数对象实现*/
//函数指针实现法
template <typename T> 
void Vector<T>::traverse(void (*visit) (T&)) { /*返回值+函数名+参数列表类型*/
  for(Rank i = 0; i < _size; ++i) {
    visit(_elem[i]);
  }
  return;
} //函数指针，只读类型或者做局部修改

//函数对象模版实现法
template <typename T> template <typename VST> 
void Vector<T>::traverse(VST visit) { //函数对象，便于全局修改
  for (Rank i = 0; i < _size; ++i) {
    visit( _elem[i] );
  }
  return;
}

//打印函数实例
template <typename T>
void print(Vector<T> &v) { 
  v.traverse( myPrint<T>() );
  cout << endl;
};//向量遍历打印

/* 测试函数
template <typename T> class myPrint { public: virtual void operator()(T &e) { std::cout << e <<" ";}};//函数对象，单个打印：通过重载操作符()实现
template <typename T> void print(Vector<T> &v) { v.traverse( myPrint<T>() ); std::cout << std::endl;};//向量遍历打印
template <typename T> class myIncrease { public: virtual void operator()(T &e) { ++e;}};//函数对象，单个加一：通过重载操作符()实现
template <typename T> void increase(Vector<T> &v) { v.traverse( myIncrease<T>() );};//向量遍历加一
template <typename T> class myDecrease { public: virtual void operator()(T &e) { --e;}};//函数对象，单个减一：通过重载操作符()实现
template <typename T> void decrease(Vector<T> &v) { v.traverse( myDecrease<T>() );};//向量遍历减一
template <typename T> class myDoubble { public: virtual void operator()(T &e) { e *= 2;}};//函数对象，单个加倍：通过重载操作符()实现
template <typename T> void doubble(Vector<T> &v) { v.traverse( myDoubble<T>() );};//向量遍历加倍
template <typename T> class mySum { public: int& Sum; mySum(int& sum) : Sum(sum){} virtual void operator()(T &e) { Sum += e;}};//通过全局变量记录求和值(可以保存状态)
template <typename T> Rank sum(Vector<T> &v) { int sumValue = 0; v.traverse( mySum<T>(sumValue)); return sumValue; };//向量整体求和

int main(void)
{
   int A[] = {1,2,3,4,5};
   Vector v2(A, end(A) - begin(A));//自己通过A的类型T*判断模板使用
   print(v2);
   v2.insert(0, 0);
   print(v2);
   increase(v2);
   print(v2);
   decrease(v2);
   print(v2);
   doubble(v2);
   print(v2);
  
   cout << sum(v2) <<endl;
}
*/

/**********************有序向量************************/
/*9.有序性检查*/

// 1.函数对象实现
template <typename T>
void checkOrder(Vector<T> &v) {
  Rank unsorted = 0;                      //逆序数计数器
  v.traverse(CheckOrder(unsorted, v[0])); //统计紧邻逆序对
  if (unsorted > 0)
    cout << "Unsorted with " << unsorted << " adjacent inversion(s)" << endl;
  else
    cout << "Sorted" << endl;

  return;
}

//2.成员函数实现
template <typename T>
Rank Vector<T>::disordered() const { // O(n)
  int unsorted = 0;
  for (int i = 1; i < _size; ++i) {
    if (_elem[i - 1] > _elem[i]) //简写为 unsorted += (_elem[ i-1 ] > _elem [i]);
      ++unsorted;
  }

  return unsorted; //返回逆序对数
}

/*10.置乱器*/
template <typename T>
void Vector<T>::unsort(Rank low, Rank high) {
  T *v = _elem + low; //可转化为数组形式了
  for (Rank i = high - low; i > 0; --i) {
    mySwap(v[i - 1], v[rand() % i]);
  }
}

/*11.有序向量去重函数*/

//高效版
template <typename T>
int Vector<T>::uniquify() { // O(n)
  int oldSize = _size;
  int i = 0;
  for (int j = 1; j < _size; ++j) {
    if (_elem[i] != _elem[j])
      _elem[++i] = _elem[j];
  }
  _size = i + 1;
  shrink(); //解除低装填因子时重复元素的内存占用

  return oldSize - _size; //返回删除元素隔个数
}
//低效版
/*
template <typename T> 
int Vector<T>::uniquify() { //O(n^2)
  int oldSize = _size;
  for (Rank i = 1; i < _size; ) {
    (_elem[i-1] == _elem[i]) ? remove(i) : ++i;
  }
  
  return oldSize - _size; //返回重复的元素个数
}
*/

/*12.有序向量查找函数*/
//统一接口
template <typename T>
Rank Vector<T>::search(T const &e, Rank low, Rank high) const {
  return (rand() % 2) ?                                 //按各50%概率调用
             binSearchC(_elem, e, low, high)            //二分查找算法，或
                      : fibSearch(_elem, e, low, high); // Fibonacci查找算法
}

////二分查找A实现（三次转向）
template <typename T> 
static Rank binSearch(T* S, T const& e, Rank low, Rank high) { //O(1.5*logn)
  while (low < high) {
    Rank mid = low + (high - low) / 2;
    
    if (e < S[mid]) high = mid; //high是尾后,访问不到[low,high),等效于mid-1
    else if (S[mid] < e) low = mid + 1;
    else return mid;
  }
  
  return -1; //查找失败，返回一个不存在的值
}

////Fibonacci查找实现
template <typename T>
static Rank fibSearch(T* S, T const& e, Rank low, Rank high) { //O(1.44logn)
  for( Fib fib(high - low);low < high; ) {       //Fibonacci数列制表（先产生一个足够大的Fib）
    while ( high - low < fib.get() ) fib.prev(); //从后往前找到合适的Fib分割点轴点——————分摊O(1)
    Rank mid = low + fib.get() - 1;

    if(e < S[mid]) high = mid - 1; //左闭右开区间
    else if (S[mid] < e) low = mid;
    else return mid;
  }

  return -1; //查找失败
}

////二分查找B实现（2次转向）
template <typename T>
static Rank binSearchB(T *S, T const &e, Rank low, Rank high) { // O(1*logn)
  while (high - low > 1) {
    Rank mid = low + (high - low) / 2;
    (e < S[mid]) ? high = mid : low = mid; //[low, mid) , [mid, high)
  }

  return (e == S[low]) ? low : -1;     //找到返回low，没找到返回-1；
  //return (e < S[low]) ? low - 1 : low; //无法判别该值e是否全大于搜索空间
}

////*****二分查找C实现（完美版）——————界桩算法不变性：S[0,low) <= e < S[high,n)
template <typename T>
static Rank binSearchC(T *S, T const &e, Rank low, Rank high) {
  while (low < high) {
    Rank mid = low + (high - low) / 2;
    if (e < S[mid])
      high = mid; //搜索空间：[low, mid)
    else
      low = mid + 1; //搜索空间：(mid, high)
    //看起来mid被空，但可以通过返回值修正
  }
  return low - 1; //返回不大于查找值e的最大值

  //后续可以出来后根据下标和与e判等操作，看是否找到
}
//递归实现完美版(此处栈溢出了，应该是内存较小，分配的栈空间少了)
template <typename T>
static Rank binSearchD(T *S, T const &e, Rank low, Rank high) {
  if (low == high) return low - 1; //递归基

  Rank mid = low + (high - low) / 2;
  return ((e < S[mid]) ? binSearch(S, e, low, mid) : binSearch(S, e, mid + 1, high));
}

////插值查找实现————闭区间 [low, high]————————快速收敛查找范围
template <typename T>
static Rank insertSearch(T* S, T const& e, Rank lo, Rank hi) {
  while (lo < hi) {
    Rank mi = lo + (hi - 1 - lo) * (e - S[lo]) / (S[hi - 1] - S[lo]); //插值节点

    if (e < S[mi])  hi = mi; //[low,mid)
    else lo = mi + 1;        //(mid, high)
  }
  return lo - 1;
}

/*13.排序算法*/
//通用接口 [low,high)<=>[0,n)
template <typename T>
void Vector<T>::sort(Rank low, Rank high) {
  switch (3) {
    case 1: bubbleSort(low, high); break;    //冒泡排序
    case 2: selectionSort(low, high); break; //选择排序(习题)
    case 3: mergeSort(low, high); break;     //归并排序
    case 4: heapSort(low, high); break;      //堆排序(C12)
    case 5: quickSort(low, high); break;     //快速排序(C14)
    default: shellSort(low, high); break;    //希尔排序(C14)
  } //随机选择算法，并可以灵活扩充新的排序算法
}

/***冒泡排序***/
////冒泡排序基础版
/*
template <typename T>
void Vector<T>::bubbleSort(Rank low, Rank high) { //最坏：O(n^2)  最好：O(n)
  --high;
  while (low < high) { //输入保证[low，high]
    for (Rank i = low; i < high; ++i) {
      if (_elem[i] > _elem[i+1]) //逆序
        mySwap(_elem[i], _elem[i+1]); //则交换——————步数消耗处
    }
    --high; //排序范围缩小
  }
}
*/

////冒泡排序提前终止版
/*
//实现接口
template <typename T>
void Vector<T>::bubbleSort(Rank low, Rank high) { //最坏：O(n^2)  最好：O(n) 但特殊情况下偏向O(n)
  while (!bubble(low,--high));
}

//扫描交换函数
template <typename T>
bool Vector<T>:: bubble(Rank low, Rank high) { 
  bool sorted = true; //整体有序标志
  for (Rank i = low; i < high; ++i) {
    if (_elem[i] > _elem[i+1]) { //取到high
      mySwap(_elem[i], _elem[i+1]);
      sorted = false;
    }
  }
  return sorted;
}
*/

//便于理解版
/*
template <typename T>
void Vector<T>::bubbleSort(Rank low, Rank high) { //最坏：O(n^2)  最好：O(n) 但特殊情况下偏向O(n)
  --high;
  for(bool sorted = false; sorted == false; ) {
    sorted = true;
    for(Rank i = low; i < high; ++i) {
      if (_elem[i] > _elem[i+1]) {
        mySwap(_elem[i], _elem[i+1]);
        sorted = false;
      }
    }
  }
}
*/

////*****冒泡排序跳跃版————跳过有序部分，仅排无序子块 转化为只排[low,last)
template <typename T>
void Vector<T>::bubbleSort(Rank low, Rank high) {   //最坏：O(n^2)  最好：O(n) 但特殊情况下偏向O(n)
  --high;                                           //[) -> []
  for (Rank last = high; low < high; high = last) { //一下子就跳到last，不用再亦步亦趋地1步1步保证已排序元素了
    for (Rank i = last = low; i < high; ++i) {      //从左往右排，last为最后一个无序标志，此时假定一开始地low为last，即扫描前认为整体无序
      if (_elem[i] > _elem[i + 1]) {
        mySwap(_elem[i], _elem[i + 1]);
        last = i;
      }
    }
  }
}
/*冒泡排序的改进主要是最外层的循环改进*/

/***归并排序***/
//归并排序算法接口（递归）
template <typename T>
void Vector<T>::mergeSort(Rank low, Rank high) { // O(nlogn)
  if (high - low == 1)                           //递归基（递归车起步，先系安全带）
    return;

  Rank mid = low + (high - low) / 2;
  mergeSort(low, mid);  //[low,mid)
  mergeSort(mid, high); //[)
  merge(low, mid, high);

  return;
}

//二路归并
template <typename T>
void Vector<T>::merge(Rank low, Rank mid, Rank high) { // O(n)
  T *A = _elem + low;
  Rank i = 0; // A就地
  Rank lb = mid - low;

  T *B = new T[lb];
  Rank j = 0; // B申请堆空间
  for (Rank k = 0; k < mid - low; ++k) {
    B[k] = A[k];
  } //逐个拷贝B

  Rank lc = high - mid;
  T *C = _elem + mid;
  Rank k = 0; // C就地

  while ((j < lb) && (k < lc)) {              //一般情况
    A[i++] = (B[j] > C[k]) ? C[k++] : B[j++]; //保证排序稳定性，相等时需要返回左侧的值
  }
  while (j < lb) { // C先排尽，还要排B；B先排尽，C可顺序就位
    A[i++] = B[j++];
  }
  delete[] B; // new和delete很耗时，可用内置栈结构来代替

  return;
}

#endif