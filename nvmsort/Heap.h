#ifndef _HEAP_H_
#define _HEAP_H_

#include "Array.h"

using namespace std;


/*template<class T>
struct Less
{
    bool operator()(const T& left, const T& right) const
    {
        return left < right;
    }
};

template<class T>
struct Greater
{
    bool operator()(const T& left, const T& right) const
    {
        return left > right;
    }
};



template<class T, class Compare=Less<T>>
class Heap
{
public:
    //无参的构造函数（系统不会给无参构造函数），开始堆是空的不需要做什么事
    Heap(vector<int> a, size_t n, double r , Pattern p)
    {
        size=n;
        _a.AllocSpace(n,r,p);//开空间
        for (size_t i = 0; i < n; i++)
        {
            _a[i]=a[i];
        }
        //建堆,找最后一个非叶子节点
        for (int i = (n - 2) / 2; i >= 0; --i)//不用size_t，因为i在这可能等于0，用size_t会死循环
        {
            AdjustDown(i);
        }
    }
    //向下调整
    void AdjustDown(int root)
    {
        Compare com;
        int parent = root;
        size_t child = parent * 2 + 1;//默认为左孩子
        while (child < size)
        {
            //选出小孩子
            //if (child+1 > _a.size() && _a[child + 1]< _a[child])
            if (child + 1 < size && com(_a[child + 1], _a[child]))
            {
                ++child;
            }

            //if (_a[child] < _a[parent])
            if (com(_a[child], _a[parent]))
            {
                SwapElem(child, parent);//交换值
                parent = child;
                child = parent * 2 + 1;
            }
            else
            {
                break;
            }
        }
    }
    //向上调整
    void AdjustUp(int child)
    {
        Compare com;
        int parent = (child - 1) / 2;
        while (parent >= 0)
        {
            //if (_a[child] < _a[parent])
            if (com(_a[child], _a[parent]))
            {
                SwapElem(parent, child);
                child = parent;
                parent = (child - 1) / 2;
            }
            else
            {
                break;
            }
        }

    }
    //最后插入
    void Push(const T&x)
    {
        size++;
        _a[size-1]=x;
        AdjustUp(size - 1);
    }
    //删除最大数
    void Pop()
    {
        //assert(!_a.empty());
        SwapElem(0, size - 1);
        size--;
        AdjustDown(0);

    }
    //取顶元素
    T& Top()
    {
        assert(!size==0);
        return _a[0];
    }
    size_t Size()
    {
        return size;
    }

    bool Empty()
    {
        return size==0;
    }

    void  HeapSort(){
	// 循环建立初始堆
	for (int i = size / 2 - 1; i >= 0; i--){
		AdjustDown(i);
	}

	// 进行n-1次循环，完成排序
	for (int i = size - 1; i > 0; i--){
		// 最后一个元素和第一元素进行交换
		int temp = _a[i];
		_a[i] = _a[0];
		_a[0] = temp;

		// 筛选 R[0] 结点，得到i-1个结点的堆
		AdjustDown(0);
	}
    }
    Array Alloc(size_t n, double r , Pattern p ){
        Array result(n,r,p);
        return result;
    }
    void SwapElem(int i,int j){
    int temp;
    temp=_a[i];
    _a[i]=_a[j];
    _a[j]=temp;
    }


private:
    Array _a(0,1.0/11,HYBRID_RAND);
    int size;
};*/
 class Heap
{
private:

public:
    Heap();
    ~Heap();
    void HeapMaxAdjust(Array &list, int parent, int length);
    void HeapMinAdjust(Array &list, int parent, int length);
    void HeapSort(Array& list,int length);
    void BuildMaxHeap(Array & list,int length);
    void BuildMinHeap(Array & list,int length);
    int HeapMaximum(Array list);
    int HeapMinimum(Array list);
    int HeapExtractMax(Array &list);
    int HeapExtractMin(Array &list);
    void MaxHeapInsert(Array &list,int key);
    void MinHeapInsert(Array &list,int key);
    void HeapIncreaseKey(Array &list,int i,int key);
    void HeapDecreaseKey(Array &list,int i,int key);
    Array Gendata(int n,double r,Pattern p);
};

Heap::Heap()
{
    
    
}

Heap::~Heap()
{
}

void Heap:: HeapMaxAdjust(Array &list, int parent, int length){
	int temp = list[parent];					// temp保存当前父节点
	int child = 2 * parent + 1;					// 先获得左孩子
	while (child < length){
		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (child + 1 < length && list[child] < list[child + 1]){
			child++;
		}

		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (temp >= list[child]){
			break;
		}

		// 把孩子结点的值赋给父结点
		list[parent] = list[child];
		// 选取孩子结点的左孩子结点,继续向下筛选
		parent = child;
		child = 2 * parent + 1;
	}
	list[parent] = temp;
}
void Heap:: HeapMinAdjust(Array &list, int parent, int length){
	int temp = list[parent];					// temp保存当前父节点
	int child = 2 * parent + 1;					// 先获得左孩子
	while (child < length){
		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (child + 1 < length && list[child] > list[child + 1]){
			child++;
		}

		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (temp <= list[child]){
			break;
		}

		// 把孩子结点的值赋给父结点
		list[parent] = list[child];
		// 选取孩子结点的左孩子结点,继续向下筛选
		parent = child;
		child = 2 * parent + 1;
	}
	list[parent] = temp;
}

void Heap::BuildMaxHeap(Array &list,int length){
    for (int i = length / 2 - 1; i >= 0; i--){
		HeapMaxAdjust(list, i, length);
	}
}
void Heap::BuildMinHeap(Array &list,int length){
    for (int i = length / 2 - 1; i >= 0; i--){
		HeapMinAdjust(list, i, length);
	}
}
void Heap:: HeapSort(Array &list,int length){
	// 循环建立初始堆
	for (int i = length / 2 - 1; i >= 0; i--){
		HeapMaxAdjust(list, i, length);
	}

	// 进行n-1次循环，完成排序
	for (int i = length - 1; i > 0; i--){
		// 最后一个元素和第一元素进行交换
		int temp = list[i];
		list[i] = list[0];
		list[0] = temp;

		// 筛选 R[0] 结点，得到i-1个结点的堆
		HeapMaxAdjust(list, 0, i);
	}
}

Array Heap:: Gendata(int n,double r,Pattern p){
    Array data(n,r,p);
    srand((int) time(0));
    for (int i=0;i<n;i++){
        data[i]=(rand()%1000000);
        data.SizePlus(1);
    }
    return data;
}

int Heap::HeapMaximum(Array list){//aim at maxheap
    int result=list[0];
    return result;
}
int Heap::HeapMinimum(Array list){//aim at minheap
    int result=list[0];
    return result;
}
int Heap::HeapExtractMax(Array &list){
    int max=list[0];
    list[0]=list[list.Size()-1];
    list.SizeMinus(1);
    HeapMaxAdjust(list,0,list.Size());
    return max;
}
int Heap::HeapExtractMin(Array &list){
    int min=list[0];
    list[0]=list[list.Size()-1];
    list.SizeMinus(1);
    HeapMinAdjust(list,0,list.Size());
    return min;
}
void Heap:: MaxHeapInsert(Array &list,int key){
    list.SizePlus(1);
    list[list.Size()-1]=(-2147483647-1);
    HeapIncreaseKey(list,list.Size()-1,key);
}
void Heap:: MinHeapInsert(Array &list,int key){
    list.SizePlus(1);
    list[list.Size()-1]=(2147483647);
    HeapDecreaseKey(list,list.Size()-1,key);
}
void Heap:: HeapIncreaseKey(Array &list,int i,int key){
    if (key<list[i]){
        cout<<" new key is smaller than current key"<<endl;
        return;
    }
    int temp;
    list[i]=key;
    while(i>1&&list[i/2]<list[i]){
        temp=list[i];
        list[i]=list[i/2];
        list[i/2]=temp;
        i=i/2;
    }
}
void Heap:: HeapDecreaseKey(Array &list,int i,int key){
    if (key>list[i]){
        cout<<" new key is bigger than current key"<<endl;
        return;
    }
    int temp;
    list[i]=key;
    while(i>1&&list[i/2]>list[i]){
        temp=list[i];
        list[i]=list[i/2];
        list[i/2]=temp;
        i=i/2;
    }
}

#endif