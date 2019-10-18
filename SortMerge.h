#ifndef _SORTMERGE_H_
#define _SORTMERGE_H_

#include "Relation.h"
#include "Heap.h"
#include "QuickSort.h"
enum JoinPattern{
    HEAP=0,
    QUICK=1
};
class SortMerge
{
private:
    /* data */
public:
template<class RelationA,class RelationB>
    SortMerge(RelationA &R, RelationB &S,JoinPattern p);
    ~SortMerge();
};

template<class RelationA,class RelationB>
SortMerge::SortMerge(RelationA &R, RelationB &S,JoinPattern p)
{
    if(p==HEAP){
        Heap heap;
        heap.HeapSort(R,R.Size());
        heap.HeapSort(S,S.Size());
    }
    else{
        QuickSort(R,0,R.Size()-1);
        QuickSort(S,0,S.Size()-1);
    }
    
    int sum=0;
    int i=0,j=0;
    while (i<R.Size()&&j<S.Size())
    {
        if (R[i].key() == S[j].key()){
            int key = R[i].key();
            int r_step = 1, s_step = 1;
            while(i < R.Size() && R[i].key() == key){
                r_step++;
                i++;
            }
                
            while(j < S.Size() && S[j].key() == key){
                s_step++;
                j++;
            }
                
            sum += r_step * s_step;
        } else if (R[i].key() < S[j].key())
            i++;
        else
            j++;
    }
    
}

SortMerge::~SortMerge()
{
}

#endif