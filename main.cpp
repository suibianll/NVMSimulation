#include "Relation.h"
#include "Heap.h"
//#include "param.h"
#include "SortMerge.h"
#include "Tools.h"
#include <sys/timeb.h>
#include <iostream>
#define Debug 1
using namespace std;

int main(int argc, char *argv[]){
    /*double ratio=1.0/10;
    Pattern p=HYBRID_SEG;
    Relation R(RSIZE,ratio,p);
    Gendata(R,RSIZE);
    QuickSort(R,0,R.Size()-1);
    for(int i=0;i<R.Size();i++){
        cout<<R[i].key()<<endl;
    }*/
    initcount();
    double ratio;
    int Rsize;
    int Ssize;
    if(Debug){
        ratio=1.0/10;
        Rsize=RSIZE*100;
        Ssize=SSIZE*100;
    }
    else
    {
        if(argc!=3){
        Message();
        exit(-1);
        }
        ratio=atof(argv[1]);
        Rsize=atoi(argv[2]);//RSIZE*1000;
        Ssize=atoi(argv[3]);//SSIZE*1000;
    }
    Pattern p=HYBRID_RAND;
    Relation R(Rsize,ratio,p);
    Relation S(Ssize,ratio,p);
    Relation R1(Rsize,ratio,p);
    Relation S1(Ssize,ratio,p);
    Gendata(R,Rsize);
    Gendata(S,Ssize);
    Copy(R,R1);
    Copy(S,S1);
    ftime(&t1);
    SortMerge sm(R,S,HEAP);
    ftime(&t2);
    cout<< (t2.time*1000+t2.millitm - t1.time*1000-t1.millitm)<<" "<<endl;
    cout<<"preads:"<<preads<<endl;
    cout<<"pwrites:"<<pwrites<<endl;
    cout<<"dreads:"<<dreads<<endl;
    cout<<"dwrites:"<<dwrites<<endl;
    initcount();
    ftime(&t1);
    SortMerge sm1(R1,S1,QUICK);
    ftime(&t2);
    cout<< (t2.time*1000+t2.millitm - t1.time*1000-t1.millitm)<<" "<<endl;
    cout<<"preads:"<<preads<<endl;
    cout<<"pwrites:"<<pwrites<<endl;
    cout<<"dreads:"<<dreads<<endl;
    cout<<"dwrites:"<<dwrites<<endl;
    return 0;
}
