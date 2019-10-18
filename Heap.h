#ifndef _HEAP_H_
#define _HEAP_H_

#include "Relation.h"


using namespace std;

 class Heap
{
private:

public:
    Heap();
    ~Heap();
    void HeapMaxAdjust(Relation &list, int parent, int length);
    void HeapMinAdjust(Relation &list, int parent, int length);
    void HeapSort(Relation& list,int length);
    void BuildMaxHeap(Relation & list,int length);
    void BuildMinHeap(Relation & list,int length);
    Record HeapMaximum(Relation list);
    Record HeapMinimum(Relation list);
    Record HeapExtractMax(Relation &list);
    Record HeapExtractMin(Relation &list);
    void MaxHeapInsert(Relation &list,Record key);
    void MinHeapInsert(Relation &list,Record key);
    void HeapIncreaseKey(Relation &list,int i,Record key);
    void HeapDecreaseKey(Relation &list,int i,Record key);
    Relation Gendata(int n,double r,Pattern p);
};

Heap::Heap()
{
    
    
}

Heap::~Heap()
{
}

void Heap:: HeapMaxAdjust(Relation &list, int parent, int length){
	Record temp = (list[parent]);					// temp保存当前父节点
	int child = 2 * parent + 1;					// 先获得左孩子
	while (child < length){
		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (child + 1 < length && list[child].key() < list[child + 1].key()){
			child++;
		}

		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (temp.key >= list[child].key()){
			break;
		}

		// 把孩子结点的值赋给父结点
		list[parent] = list[child];
		// 选取孩子结点的左孩子结点,继续向下筛选
		parent = child;
		child = 2 * parent + 1;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	list[parent]= temp;
}
void Heap:: HeapMinAdjust(Relation &list, int parent, int length){
	Record temp = list[parent];					// temp保存当前父节点
	int child = 2 * parent + 1;					// 先获得左孩子
	while (child < length){
		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (child + 1 < length && list[child].key() > list[child + 1].key()){
			child++;
		}

		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (temp.key <= list[child].key()){
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

void Heap::BuildMaxHeap(Relation &list,int length){
    for (int i = length / 2 - 1; i >= 0; i--){
		HeapMaxAdjust(list, i, length);
	}
}
void Heap::BuildMinHeap(Relation &list,int length){
    for (int i = length / 2 - 1; i >= 0; i--){
		HeapMinAdjust(list, i, length);
	}
}
void Heap:: HeapSort(Relation &list,int length){
	// 循环建立初始堆
	for (int i = length / 2 - 1; i >= 0; i--){
		HeapMaxAdjust(list, i, length);
	}

	// 进行n-1次循环，完成排序
	for (int i = length - 1; i > 0; i--){
		// 最后一个元素和第一元素进行交换
		Record temp = list[i];
		list[i] = list[0];
		list[0] = temp;

		// 筛选 R[0] 结点，得到i-1个结点的堆
		HeapMaxAdjust(list, 0, i);
	}
}

Relation Heap:: Gendata(int n,double r,Pattern p){
    Relation data(n,r,p);
    srand((int) time(0));
    for (int i=0;i<n;i++){
        Record r;
        r.key=(rand()%1000000);
        r.value=(rand()%1000000);
        data[i]=r;
        data.SizePlus(1);
    }
    return data;
}

Record Heap::HeapMaximum(Relation list){//aim at maxheap
    Record result=list[0];
    return result;
}
Record Heap::HeapMinimum(Relation list){//aim at minheap
    Record result=list[0];
    return result;
}
Record Heap::HeapExtractMax(Relation &list){
    Record max=list[0];
    list[0]=list[list.Size()-1];
    list.SizeMinus(1);
    HeapMaxAdjust(list,0,list.Size());
    return max;
}
Record Heap::HeapExtractMin(Relation &list){
    Record min=list[0];
    list[0]=list[list.Size()-1];
    list.SizeMinus(1);
    HeapMinAdjust(list,0,list.Size());
    return min;
}
void Heap:: MaxHeapInsert(Relation &list,Record key){
    list.SizePlus(1);
    Record r={(-2147483647-1),0};
    list[list.Size()-1]=r;
    HeapIncreaseKey(list,list.Size()-1,key);
}
void Heap:: MinHeapInsert(Relation &list,Record key){
    list.SizePlus(1);
    Record r={(-2147483647-1),0};
    list[list.Size()-1]=r;
    HeapDecreaseKey(list,list.Size()-1,key);
}
void Heap:: HeapIncreaseKey(Relation &list,int i,Record key){
    if (key.key<list[i].key()){
        cout<<" new key is smaller than current key"<<endl;
        return;
    }
    Record temp;
    list[i]=key;
    while(i>1&&list[i/2].key()<list[i].key()){
        temp=list[i];
        list[i]=list[i/2];
        list[i/2]=temp;
        i=i/2;
    }
}
void Heap:: HeapDecreaseKey(Relation &list,int i,Record key){
    if (key.key>list[i].key()){
        cout<<" new key is bigger than current key"<<endl;
        return;
    }
    Record temp;
    list[i]=key;
    while(i>1&&list[i/2].key()>list[i].key()){
        temp=list[i];
        list[i]=list[i/2];
        list[i/2]=temp;
        i=i/2;
    }
}

#endif