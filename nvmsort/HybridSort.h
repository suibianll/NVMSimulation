#ifndef _HYBRIDSORT_H_
#define _HYBRIDSORT_H_

#include "Array.h"
#include "Heap.h"
#include <vector>
const double x=0.2;
Heap heap;
void MergeSort(Array &Output, int start,int M,Array TempMem,vector<int>PointerRun){
    Array heapbuffer(M,0,PURE_DRAM);
    vector<int> pointer;
    pointer.assign(PointerRun.begin(),PointerRun.end());
    int i,j,t;
    int count=0;
    while(count<TempMem.Size()){
        for(i=0;i<PointerRun.size()-1;i++){
            if(pointer[i]<PointerRun[i+1]){
                if(heapbuffer.Size()==0){
                    heapbuffer[0]=TempMem[pointer[i]++];
                    heapbuffer.SizePlus(1);
                    count++;
                }
                else{
                    if(heapbuffer.Size()==M){
                        t=heap.HeapExtractMin(heapbuffer);
                        Output[start]=t;
                        start++;
                        Output.SizePlus(1);
                    }
                    heap.MinHeapInsert(heapbuffer,TempMem[pointer[i]]);
                    pointer[i]++;
                    count++;
                }
            }
        }
    }
    
}
void HybridSort(Array&T, int M){
    int s=x*M;
    int r=M-s;
    Array Output(T.Size(),0,PURE_NVM);
    Array  TempMem(T.Size()-s,0,PURE_NVM);//nvm to temporarily store the sorted run.
    Array Rs(s,0,PURE_DRAM);
    Array Rr_current(r,0,PURE_DRAM);
    Array Rr_next(r,0,PURE_DRAM);
    
    vector<int> PointerRun;//pinter array pointing to the start of each run.
    PointerRun.push_back(0);PointerRun.push_back(0);
    int i,t,m,n;
    int round=1;
    for(i=0;i<s;i++){
        Rs[i]=T[i];
        Rs.SizePlus(1);
    }
        
    heap.BuildMaxHeap(Rs,s);
    for(i=s;i<T.Size();i++){
        t=T[i];
      //  int temp=heap.HeapMaximum(Rs);
        if(t<Rs[0]){
            m=heap.HeapExtractMax(Rs);
            heap.MaxHeapInsert(Rs,t);
            t=m;
        }
        if(Rr_current.Size()+Rr_next.Size()<r){
            Rr_current[Rr_current.Size()]=t;
            Rr_current.SizePlus(1);
            if(Rr_current.Size()==r)
                heap.BuildMinHeap(Rr_current,Rr_current.Size());
        }
        else{
            n=heap.HeapExtractMin(Rr_current);
            TempMem[PointerRun[round]++]=n;
            TempMem.SizePlus(1);
            if(t>=n)
                heap.MinHeapInsert(Rr_current,t);
            else
                heap.MinHeapInsert(Rr_next,t);
            if(Rr_current.Size()==0){
                PointerRun.push_back(PointerRun[round]);
                round++;//close current run and start new run
                for(int j=0;j<Rr_next.Size();j++){
                    Rr_current[j]=Rr_next[j];
                }
                Rr_current.SizePlus(Rr_next.Size());
                Rr_next.SizeMinus(Rr_next.Size());
                heap.BuildMinHeap(Rr_current,Rr_current.Size());
            } 
        }
    }
    heap.HeapSort(Rs,Rs.Size());
    for(i=0;i<s;i++){
        Output[i]=Rs[i];
        Output.SizePlus(1);
    }
        
    int Rr_CurrentSIze=Rr_current.Size();
    int Rr_nextSize=Rr_next.Size();
    for(i=0;i<Rr_CurrentSIze-1;i++){
        TempMem[PointerRun[round]++]=heap.HeapExtractMin( Rr_current);
        TempMem.SizePlus(1);
    }
    PointerRun.push_back(PointerRun[round]);
    round++;
    for(i=0;i<Rr_nextSize-1;i++){
        TempMem[PointerRun[round]++]=heap.HeapExtractMin( Rr_next);
        TempMem.SizePlus(1);
    }
   MergeSort(Output,s,M,TempMem,PointerRun);
    //return Output;
}


#endif