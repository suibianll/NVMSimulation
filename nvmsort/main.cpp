/*
 *   Copyright (C) 2019 Yongping Luo <ypluo18@qq.com>
*/

#include "Array.h"
#include "Heap.h"
#include "HybridSort.h"
#include "QuickSort.h"
#include <fstream>
#include <ctime>
#include <sys/time.h>
#include <sys/timeb.h>
uint64_t dreads = 0, preads = 0;
uint64_t dwrites = 0, pwrites = 0;

using std::cout;
using std::endl;
using std::string;

clock_t start;
clock_t endtime;
const int TEST_NUM = 10000;
struct timeb t1, t2;
int GetDataNum(string filepath){
    int num=0;
    FILE *fp;
	fp = fopen(filepath.c_str(), "r");
    char buffer[20];
    vector<int> result;
    while((fgets(buffer, sizeof(buffer), fp) != NULL)){//
        num++;
    }
    return num;
}
void ReadData(Array &array,string filepath,double ratio,Pattern p){//load data from the file
    FILE *fp;
	fp = fopen(filepath.c_str(), "r");
    //ifstream fin(filepath);
    char num[20];
    vector<int> result;
    while((fgets(num, sizeof(num), fp) != NULL)){//
        int temp=atoi(num);
        result.push_back(temp);
    }

    //array=new Array (result.size(),ratio,p);
    for(int i=0;i<result.size();i++){
        array[i]=result[i];
    }
    array.SizePlus(result.size());
}

void PrintOut(){
    cout << dreads <<" " << dwrites <<" "<< preads << " " << pwrites << endl;
}

void initcount(){
    dreads = 0, preads = 0;
    dwrites = 0, pwrites = 0;
}

int main(int argc, char *argv[]) {
   // string path = "/mnt/pmemdir";
   // pmem_init(path);
    
    Heap heap;
    double ratio=atof(argv[1]);
    string datapath=argv[2];
    Pattern p=HYBRID_SEG;
    int datasize=GetDataNum(datapath);
    //cout<<ratio<<endl;

    //cout << "==================start of the test==================" << endl;
  
    /*HeapSort using Hybrid random nvm/dram*/
    p=HYBRID_RAND;
    initcount();
    Array testdata(datasize,ratio,p);
    ReadData(testdata,datapath,ratio,p);
    start=clock();
    ftime(&t1);
    heap.HeapSort(testdata,testdata.Size());
    ftime(&t2);
    endtime=clock();
    cout<< (t2.time*1000+t2.millitm - t1.time*1000-t1.millitm)<<" ";
    //delete testdata;
    //cout<<"HeapSort using Hybrid random nvm/dram"<<endl;
    //cout<<(double)(endtime-start)/CLOCKS_PER_SEC*1000<<" ";
    //cout << "DRAM Reads: " << dreads << " DRAM Writes: " << dwrites << endl;
    //cout << "NVM Reads: " << preads << " NVM Writes: " << pwrites << endl;
    //cout<<" "<<endl;*/
    PrintOut();
    //testdata.~Array();
    /*HeapSort using Hybrid seg nvm/dram*/
    p=HYBRID_SEG;
    initcount();
    Array testdata1(datasize,ratio,p);
    ReadData(testdata1, datapath,ratio,p);
    start=clock();
    ftime(&t1);
    heap.HeapSort(testdata1,testdata1.Size());
    ftime(&t2);
    endtime=clock();
    cout<< (t2.time*1000+t2.millitm - t1.time*1000-t1.millitm)<<" ";
    //delete testdata1;
    //cout<<"HeapSort using Hybrid seg nvm/dram"<<endl;
    //cout<<(double)(endtime-start)/CLOCKS_PER_SEC*1000<<" ";
    //cout << "DRAM Reads: " << dreads << " DRAM Writes: " << dwrites << endl;
    //cout << "NVM Reads: " << preads << " NVM Writes: " << pwrites << endl;
    //cout<<" "<<endl;
    PrintOut();
    //testdata1.~Array();
    /*QuickSort using Hybrid random nvm/dram*/
    p=HYBRID_SEG;
    initcount();
    Array testdata2(datasize,ratio,p);
    ReadData(testdata2,datapath,ratio,p);
    start=clock();
    ftime(&t1);
    QuikSort(testdata2,0,testdata2.Size()-1);
    ftime(&t2);
    endtime=clock();
    cout<< (t2.time*1000+t2.millitm - t1.time*1000-t1.millitm)<<" ";
    //delete testdata2;
    //cout<<"QuikSort using Hybrid random nvm/dram"<<endl;
    //cout<<(double)(endtime-start)/CLOCKS_PER_SEC*1000<<" ";
    //cout << "DRAM Reads: " << dreads << " DRAM Writes: " << dwrites << endl;
    //cout << "NVM Reads: " << preads << " NVM Writes: " << pwrites << endl;
    //cout<<" "<<endl;
    
   PrintOut();
    //testdata2.~Array();
    /*QuickSort using Hybrid segnvm/dram
    p=HYBRID_SEG;
    initcount();
    Array testdata3=ReadData(datapath,ratio,p);
    start=clock();
    ftime(&t1);
    QuikSort(testdata3,0,testdata3.Size()-1);
    ftime(&t2);
    endtime=clock();
    cout<< (t2.time*1000+t2.millitm - t1.time*1000-t1.millitm)<<" ";
    //cout<<"QuickSort using Hybrid segnvm/dram"<<endl;
   // cout<<(double)(endtime-start)/CLOCKS_PER_SEC*1000<<" ";
    //cout << "DRAM Reads: " << dreads << " DRAM Writes: " << dwrites << endl;
    //cout << "NVM Reads: " << preads << " NVM Writes: " << pwrites << endl;
    //cout<<" "<<endl;
    
   PrintOut();
    //testdata3.~Array();
    /*HybridSort*/
    p=HYBRID_SEG;
    initcount();
    //cout<<"HybridSort"<<endl;
    Array testdata4(datasize,ratio,p);
    ReadData(testdata4,datapath,ratio,p);
    start=clock();
    ftime(&t1);
    HybridSort(testdata4,testdata4.Size()*ratio);
    ftime(&t2);
    endtime=clock();
    cout<< (t2.time*1000+t2.millitm - t1.time*1000-t1.millitm)<<" ";
    //delete testdata4;
    //cout<<(double)(endtime-start)/CLOCKS_PER_SEC*1000<<" ";
    //cout << "DRAM Reads: " << dreads << " DRAM Writes: " << dwrites << endl;
    //cout << "NVM Reads: " << preads << " NVM Writes: " << pwrites << endl;*/
    PrintOut();

    //cout << "==================end of the test==================" << endl;
    return 0;
}
