/*
    some functions
*/
#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <iostream>
#include "Relation.h"
using namespace std;

#include <unistd.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

struct timeb t1, t2;
typedef struct timeval TC_t;
#define TC_START(start) gettimeofday(&start,NULL)
#define TC_END(End) gettimeofday(&end,NULL)
#define TC_CALC(start, end) (end.tv_sec - start.tv_sec + (double)(end.tv_usec - start.tv_usec)/1000000)


void Message(){
    cout<<"you should provide 3 parameters:ratio,Rsize,Ssize"<<endl;
}

void Gendata(Relation &data,int size){
    //Relation data(n,r,p);
    srand((int) time(0));
    for (int i=0;i<size;i++){
        Record r;
        r.key=(rand()%1000000);
        r.value=(rand()%1000000);
        data[i]=r;
        data.SizePlus(1);
    }
    return ;
}
void initcount(){
    dreads = 0, preads = 0;
    dwrites = 0, pwrites = 0;
}

void Copy(Relation &src, Relation &dst){//copy relation src to dst
    if(src.SpaceSize()!=dst.SpaceSize()){
        cout<<"Space not enough"<<endl;
    }
    int i;
    for(i=0;i<src.Size();i++){
        dst[i]=src[i];
        dst.SizePlus(1);
    }
}

#endif