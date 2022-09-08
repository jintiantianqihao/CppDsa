#ifndef VECTOR_H
#define VECTOR_H

using Rank = int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

Rank myMax(Rank a, Rank b) {
   return (a > b) ? a : b;
}

template <typename T> class Vector { //向量模板类

protected:
    Rank _size; Rank _capacity;  T* _elem; //规模(逻辑存储空间)、容量(物理存储空间)、数据区
    void copyFrom(T const* A, Rank low, Rank high); //复制数组区间A[low, high)
    void expand(); //空间不足时扩容
    void shrink(); //装填因子过小时压缩
    bool bubble(Rank low, Rank high); //扫描交换
    void bubbleSort(Rank low, Rank high); //起泡排序算法
    Rank maxItem(Rank low, Rank high); //选取最大元素
    void selectionSort(Rank low, Rank high); //选择排序算法
    void merge(Rank low, Rank mi, Rank high); //归并算法
    void mergeSort(Rank low, Rank high); //归并排序算法
    void heapSort(Rank low, Rank high); //堆排序（稍后结合完全堆讲解）
    Rank partition(Rank low, Rank high); //轴点构造算法
    void quickSort(Rank low, Rank high); //快速排序算法
    void shellSort(Rank low, Rank high); //希尔排序算法

public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
    } //s<=c
    Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
    Vector(T const* A, Rank low, Rank high) { copyFrom(A, low, high); } //区间
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
    Vector(Vector<T> const& V, Rank low, Rank high) { copyFrom(V._elem, low, high); } //区间

 // 析构函数
    ~Vector() { delete[] _elem; } //释放内部空间

// 只读访问接口
    Rank size() const { return _size; } //规模    
    bool empty() const { return !_size; } //判空
    Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
    Rank find(T const& e, Rank low, Rank high) const; //无序向量区间查找
    Rank search(T const& e) const { return (0 >= _size) ? -1 : search(e, 0, _size); }//有序向量整体查找
    Rank search(T const& e, Rank low, Rank high) const; //有序向量区间查找

 // 可写访问接口
    T& operator[] (Rank r); //重载下标操作符，可以类似于数组形式引用各元素
    const T& operator[] (Rank r) const; //仅限于做右值的重载版本
    Vector<T>& operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
    T remove(Rank r); //删除秩为r的元素
    int remove(Rank low, Rank high); //删除秩在区间[low, high)之内的元素
    Rank insert(Rank r, T const& e); //插入元素
    Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank low, Rank high); //对[low, high)排序
    void sort() { sort(0, _size); } //整体排序
    void unsort(Rank low, Rank high); //对[low, high)置乱
    void unsort() { unsort(0, _size); } //整体置乱
    Rank deduplicate(); //无序去重
    Rank uniquify(); //有序去重

 // 遍历
    void traverse(void (*) (T&)); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
}; //Vector


#endif