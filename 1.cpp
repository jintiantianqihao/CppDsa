#include <iostream>
#include <vector>
using namespace std;

using Rank = int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

Rank myMax(Rank a, Rank b) {
   return (a > b) ? a : b;
}

template <typename T> class Vector { //向量模板类
protected:
   Rank _size; Rank _capacity;  T* _elem; //规模、容量、数据区
   void copyFrom ( T const* A, Rank lo, Rank hi ); //复制数组区间A[lo, hi)
   void expand(); //空间不足时扩容
   void shrink(); //装填因子过小时压缩
   bool bubble ( Rank lo, Rank hi ); //扫描交换
   void bubbleSort ( Rank lo, Rank hi ); //起泡排序算法
   Rank maxItem ( Rank lo, Rank hi ); //选取最大元素
   void selectionSort ( Rank lo, Rank hi ); //选择排序算法
   void merge ( Rank lo, Rank mi, Rank hi ); //归并算法
   void mergeSort ( Rank lo, Rank hi ); //归并排序算法
   void heapSort ( Rank lo, Rank hi ); //堆排序（稍后结合完全堆讲解）
   Rank partition ( Rank lo, Rank hi ); //轴点构造算法
   void quickSort ( Rank lo, Rank hi ); //快速排序算法
   void shellSort ( Rank lo, Rank hi ); //希尔排序算法

public:

// 构造函数
   Vector ( int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0 ) //容量为c、规模为s、所有元素初始为v
   { _elem = new T[_capacity = c]; for ( _size = 0; _size < s; _elem[_size++] = v ); } //s<=c
   Vector ( T const* A, Rank n ) { copyFrom ( A, 0, n ); } //数组整体复制
    Vector ( T const* A, Rank lo, Rank hi ) { copyFrom ( A, lo, hi ); } //区间
    Vector ( Vector<T> const& V ) { copyFrom ( V._elem, 0, V._size ); } //向量整体复制
    Vector ( Vector<T> const& V, Rank lo, Rank hi ) { copyFrom ( V._elem, lo, hi ); } //区间

 // 析构函数
    ~Vector() { delete [] _elem; } //释放内部空间

// 只读访问接口
    Rank size() const { return _size; } //规模    
    bool empty() const { return !_size; } //判空
    Rank find ( T const& e ) const { return find ( e, 0, _size ); } //无序向量整体查找
    Rank find ( T const& e, Rank lo, Rank hi ) const; //无序向量区间查找
    Rank search ( T const& e ) const //有序向量整体查找
   { return ( 0 >= _size ) ? -1 : search ( e, 0, _size ); }
    Rank search ( T const& e, Rank lo, Rank hi ) const; //有序向量区间查找
    Rank disordered() const;

 // 可写访问接口
    T& operator[] ( Rank r ); //重载下标操作符，可以类似于数组形式引用各元素
    const T& operator[] ( Rank r ) const; //仅限于做右值的重载版本
    Vector<T> & operator= ( Vector<T> const& ); //重载赋值操作符，以便直接克隆向量
    T remove ( Rank r ); //删除秩为r的元素
    int remove ( Rank lo, Rank hi ); //删除秩在区间[lo, hi)之内的元素
    Rank insert ( Rank r, T const& e ); //插入元素
    Rank insert ( T const& e ) { return insert ( _size, e ); } //默认作为末元素插入
    void sort ( Rank lo, Rank hi ); //对[lo, hi)排序
    void sort() { sort ( 0, _size ); } //整体排序
    void unsort ( Rank lo, Rank hi ); //对[lo, hi)置乱
    void unsort() { unsort ( 0, _size ); } //整体置乱
    Rank deduplicate(); //无序去重
    Rank uniquify(); //有序去重

 // 遍历
    void traverse ( void (* ) ( T& ) ); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST> void traverse (VST); //遍历（使用函数对象，可全局性修改）

 }; //Vector
template <typename T> 
static Rank binSearch(T* S, T const& e, Rank low, Rank high);
template <typename T>
static Rank fibSearch(T* S, T const& e, Rank low, Rank high);
template <typename T> 
static Rank binSearchB(T* S, T const& e, Rank low, Rank high);
template <typename T> 
static Rank binSearchC(T* S, T const& e, Rank low, Rank high);
template <typename T>
static Rank insertSearch(T* S, T const& e, Rank lo, Rank hi);

template <typename T> void Vector<T>::copyFrom( T const* A, Rank low, Rank high)
{
  _elem = new T[_capacity = 2*(high - low)];//分配空间，并为_capacity赋值
  _size = 0;//大小初始化，规模清零
  while (low < high)
    _elem[_size++] = A[low++];//复制到_elem[0,high - low)

  return;
}

template <typename T> void Vector<T>::expand() { //向量空间不足时扩容
  if (_size < _capacity) return; //尚未满员时，不必扩容
  _capacity = myMax(_capacity, DEFAULT_CAPACITY); //往大处扩容

  T* oldElem = _elem;
  _elem = new T[_capacity << 1]; //移位法扩大一倍
  for (Rank i = 0; i < _size; ++i) {
    _elem[i] = oldElem[i];
  }
  delete[] oldElem; //*释放原有空间

  return;
} //得益于向量封装，尽管扩容后数据区的物理地址有所改变，*但不会出现"野指针"

template <typename T> T& Vector<T>::operator[] (Rank r) { //重载下标运算符
  return _elem[r]; //0 <= r <= _size
}

template <typename T> const T& Vector<T>::operator[](Rank r) const { //只读仅仅做右值的重载下标符
  return _elem[r]; //0 <= r <= _size
}

/*4.插入函数*/
template <typename T> Rank Vector<T>::insert(Rank r, T const &e) { // 0<=r<=_size
  expand(); //如有必要则扩容

  for (Rank i = _size; i > r; --i) { //O(n - r)：从后往前
    _elem[i] = _elem[i - 1];
  }
  _elem[r] = e; //插入新元素

  ++_size; //容量更新
  return r; //返回秩
}
/*5.删除函数*/
//缩容函数
template <typename T> void Vector<T>::shrink() { //装填因子过小时压缩空间，提高空间利用效率
  if (_capacity < DEFAULT_CAPACITY << 1) return; //保证不收缩到默认容量之下
  if (_size << 2 > _capacity) return; //保证装填因子_size/_capacity在25%以下才进行缩容

  T *oldElem = _elem; //另开辟新的动态内存空间
  _elem = new T[_capacity >> 1]; //容量减半
  for (Rank i = 0; i < _size; ++i) {// O(n)
    _elem[i] = oldElem[i];
  }
  delete [] oldElem; //释放原空间
  
  return;
}

//区间删除
template <typename T> int Vector<T>::remove(Rank low, Rank high) { //O(n -high)
  if (low == high) return 0; //为了提升算法效率，单独处理特殊简易的退化情况
  while (high < _size) {
    _elem[low++] = _elem[high++]; //O(high - low) : [high,_size)顺次前移，low更新到low + _size - high = n - (high - low)
  }
  _size = low; //更新规模
  shrink(); //如有必要则缩容,都是有this指针的

  return high - low; //返回被删除元素个数  
}

//单元素删除：直接调用区间删除的特例版即可
template <typename T> T Vector<T>::remove(Rank r) { //O(n-r)
  T e = _elem[r]; //备份，用作返回值
  remove(r, r + 1);
  return e; //返回被删除的元素
}//若反之利用递归调用单元素删除实现区间删除，则会遭遇与循环移位相同的窘境：大量单次无效的前移，不能一步到位，从而导致时间复杂度为O(n^2)而非O(n)

/*6.查找函数*/
template <typename T> Rank Vector<T>::find(T const &e, Rank low, Rank high) const { //O(high - low), 0<=low<=high<=_size
  while ((low < high--) && (e != _elem[high]));//while ((e != _elem[--high]) && (low <= high)); (错误，可能在最后越下界访问到_elem[-1],发生段错误)
  return high; //返回值小于low时则查找失败；否则命中目标的秩，多值情况返回秩最大者  
}

/*7.无序去重函数*/
template <typename T> Rank Vector<T>::deduplicate() {
  Rank oldSize = _size;
  for (Rank i = 1; i < _size; ) {
    (find(_elem[i], 0, i) < 0) ? ++i : remove(i); //前缀钟寻找雷同者；没找到，继续找下一个；找到，删除该重复元素，并更新规模
  }
  
  return oldSize - _size; //返回所有重复元素的个数  
}

/*8.遍历函数————利用函数对象实现*/
//函数指针实现法
template <typename T> void Vector<T>::traverse(void (*visit) (T&)) { /*返回值+函数名+参数列表类型*/
  for(Rank i = 0; i < _size; ++i) {
    visit(_elem[i]);
  }
  return;
} //函数指针，只读类型或者做局部修改

//函数对象模版实现法
template <typename T> template <typename VST> void Vector<T>::traverse(VST visit) { //函数对象，便于全局修改（此处传引用和传值均可，因为只涉及调用函数，不涉及修改函数对象）
  for (Rank i = 0; i < _size; ++i) {
    visit( _elem[i] );
  }
  return;
}

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


template <typename T> class CheckOrder { //函数对象：判断一个T类对象是否局部有序
 public:
  T pre;
  Rank& u; //此处声明了类型成员，定义需要在构造函数生成的过程中进行，并且这里必须有自定义构造函数，而不能依赖生成构造函数

  CheckOrder(Rank& unsorted, T& first) : pre(first),u(unsorted) {} //类构造函数，初始化引用型对象u;
  virtual void operator()(T& e) { if (pre > e) ++u; pre = e;} //重载函数：找寻逆序对
};

template <typename T> void checkOrder(Vector<T> &v) { //执行函数
  Rank unsorted = 0; //逆序数计数器
  v.traverse( CheckOrder(unsorted,v[0]) ); //统计紧邻逆序对
  if(unsorted > 0)
    cout << "Unsorted with " << unsorted << " adjacent inversion(s)" << endl;
  else
    cout << "Sorted" << endl;

  return;
}

//2.成员函数实现
template <typename T> Rank Vector<T>::disordered() const {
  int unsorted = 0;
  for (int i = 1; i < _size; ++i) {
    (_elem[ i-1 ] <= _elem[i]) ? : ++unsorted; //简写为 unsorted += (_elem[ i-1 ] > _elem [i]);
  }
  if(unsorted > 0)
    cout << "Unsorted with " << unsorted << " adjacent inversion(s)" << endl;
  else
    cout << "Sorted" << endl;

  return unsorted;//返回逆序对数
}
/*
template <typename T> int Vector<T>::uniquify() { //O(n^2)
  int oldSize = _size;
  for (Rank i = 1; i < _size; ) {
    (_elem[i-1] == _elem[i]) ? remove(i) : ++i;
  }
  
  return oldSize - _size; //返回重复的元素个数
}
*/

template <typename T> int Vector<T>::uniquify() { //O(n)
  int oldSize = _size;
  int i = 0;
  for (int j = 1; j < _size; ++j) {
    if (_elem[i] != _elem[j])
      _elem[++i] = _elem[j];
  }
  _size = i + 1;
  shrink(); //解除低装填因子时重复元素的内存占用

  return oldSize -_size; //返回删除元素隔个数
}

/*11.有序向量查找函数*/
//统一接口
template <typename T> 
Rank Vector<T>::search(T const& e, Rank low, Rank high) const {
  return (1) ?  //按各50%概率调用
    insertSearch(_elem, e, low, high)  //二分查找算法，或
  : fibSearch(_elem, e, low, high); //Fibonacci查找算法
}

//二分查找A实现
template <typename T> 
static Rank binSearch(T* S, T const& e, Rank low, Rank high) {
  while (low < high) {
    Rank mid = low + (high - low) / 2;
    
    if (e < S[mid]) high = mid; //high是尾后,访问不到[low,high),等效于mid-1
    else if (S[mid] < e) low = mid + 1;
    else return mid; //查找成功，返回下标
  }
  
  return -1; //查找失败，返回一个不存在的值
}

//Fibonacci查找实现
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

template <typename T>
static Rank fibSearch(T* S, T const& e, Rank low, Rank high) {
  cout << "fib:" <<endl;
  for( Fib fib(high - low);low < high;) { //Fibonacci数列制表（先产生一个足够大的Fib）
    while ( high - low < fib.get() ) fib.prev(); //从后往前找到合适的Fib分割点轴点——————分摊O(1)
    Rank mid = low + fib.get() - 1;

    if(e < S[mid]) high = mid - 1; //左闭右开区间
    else if (S[mid] < e) low = mid;
    else return mid;
  }

  return -1; //查找失败
}

//二分查找B实现（2次转向）
template <typename T> 
static Rank binSearchB(T* S, T const& e, Rank low, Rank high) {
  while (high - low > 1) {
    Rank mid = low + (high - low) / 2;

    (e < S[mid]) ? high = mid : low = mid;
  }
  
  return (e == S[low]) ? low : -1; //找到返回low，没找到返回-1；
}

//二分查找C实现(2次转向 + 语义约定：返回不大于e的秩最大值)
template <typename T>
static Rank binSearchC(T* S, T const& e, Rank low, Rank high) {
  while (low < high)
  {
    Rank mid = low + (high - low) / 2;
    if (e < S[mid]) high = mid; //搜索空间：[low, mid)
    else  low = mid + 1; //搜索空间：(mid, high)
    //看起来mid被空，但可以通过返回值修正
  }
  return low - 1; //返回不大于查找值e的最大值
}

//插值查找实现————闭区间 [low, high]：关键字分布不均，如{1，2，20000，20003}这样就不适合插值查找
template <typename T>
static Rank insertSearch(T* S, T const& e, Rank lo, Rank hi) {
  --hi;
  while (lo <= hi) {
    Rank mi = lo + (hi - lo) * (e - S[lo]) / (S[hi] - S[lo]); //插值节点---有奇点lo == hi

    if (e < S[mi])  hi = mi - 1; //[low,mid-1] <=> [low,mid)
    else if (S[mi] < e)  lo = mi + 1; //[mid+1,low] <=> (mid,high]
    else  return mi;
  }

  return -1;
}//有局部病态现象，陷入死循环中，例如 e = 4,{1,3,6,8,9}


int main(void)
{
  int A[] = {1,3,6,8,9};
  Vector v2(A, end(A) - begin(A)); //自己通过A的类型T*判断模板使用
  checkOrder(v2);
  v2.disordered();
  int k;
  cin >> k;
  cout << v2.search(k) << endl;
  print(v2);

}
