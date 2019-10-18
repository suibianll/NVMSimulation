#ifndef _QUICKSORT_H_
#define _QUICKSORT_H_
#include "Relation.h"

int Partition(Relation &A, int p, int r){
    Record x=A[r];
    int i=p-1;
    for(int j=p;j<r;j++){
        if(A[j].key()<=x.key){
            i++;
            Record temp=A[i];
            A[i]=A[j];
            A[j]=temp;
        }
    }
    Record temp=A[i+1];
    A[i+1]=A[r];
    A[r]=temp;
    return i+1;
}

void QuickSort(Relation &A,int p,int r){
    if(p<r){
        int q=Partition(A,p,r);
        QuickSort(A,p,q-1);
        QuickSort(A,q+1,r);
    }
}




#endif 