#include <iostream>
#include <queue>
#include <set>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fstream>
//#include <types.h>
//#include <ex_common.h>
#include <libpmemobj.h>

using namespace std;
#define TOID_ARRAY(x) TOID(x)
#define COUNT_OF(x) (sizeof(x) / sizeof(x[0]))
#define MAX_BUFFLEN 30
#define MAX_TYPE_NUM 8
//#define CREATE_MODE_RW (S_IWUSR | S_IRUSR)

POBJ_LAYOUT_BEGIN(array);
POBJ_LAYOUT_TOID(array, struct array_elm);
POBJ_LAYOUT_TOID(array, int);
POBJ_LAYOUT_TOID(array, PMEMoid);
POBJ_LAYOUT_TOID(array, TOID(struct array_elm));
POBJ_LAYOUT_TOID(array, struct array_info);
POBJ_LAYOUT_END(array);

static PMEMobjpool *pop;
enum array_types {
	UNKNOWN_ARRAY_TYPE,
	INT_ARRAY_TYPE,
	PMEMOID_ARRAY_TYPE,
	TOID_ARRAY_TYPE,
	MAX_ARRAY_TYPE
};

struct array_elm {
	int id;
};

struct array_info {
	char name[MAX_BUFFLEN];
	size_t size;
	enum array_types type;
	PMEMoid array;
};

typedef struct element{
    int value;
    int subarray;
    bool end;
    bool   operator<(const element&a) const{
        return value<a.value;
    }
}Element;
static int M=16;//the size of buffer
static int B=8;//the size of a block
static int k=2;//the write /read
int L=k*M/B;//the partition num
int lastV;
vector<int> I;//the pointer of block
vector<int> RestNum;//record the num of element in each block 
set<Element> Q;//the queue in cache
TOID(int) PersistantA;//the array to be sorted 
TOID(int) PersistantO;//the output array

vector<int> ReadData(string filepath){//load data from the file
    ifstream fin(filepath);
    string num;
    vector<int> result;
    while(getline(fin,num)){//
        int temp=atoi(num.data());
        result.push_back(temp);
    }
    return result;
}

void ProcessBlock(TOID(int)A,int start,int i, int subarraysize){
    Element e;
    if(I[i]==(i+1)*subarraysize) return;
    int j=0;
    for(j=I[i];j<(i+1)*subarraysize&&j<I[i]+B;j++){
        e.value=D_RO(A)[start+j];
        e.subarray=i;
        
        if(RestNum[i]==0)//the element of the block  is used up
            e.end=true;
        else
        {
            e.end=false;
        }
        if(Q.empty()&&e.value>lastV){
            Q.insert(e);
            RestNum[i]--;
        }
        else if(e.value>lastV){
            if(Q.size()==M&&e.value<(*(--Q.end())).value) {//the queue is full,,but the element is in the range
                Element temp=*(--Q.end());
                RestNum[temp.subarray]++;//
                Q.erase(temp);
                Q.insert(e);
                RestNum[i]--;   
            }
            else if (Q.size()<M){
                Q.insert(e);
                RestNum[i]--;
            }
           
        }
    }
}

static PMEMoid  alloc_int(size_t size, vector<int> A)//alloc an array 
{
	TOID(int) array;
	/*
	 * To allocate persistent array of simple type is enough to allocate
	 * pointer with size equal to number of elements multiplied by size of
	 * user-defined structure.
	 */
	POBJ_ALLOC(pop, &array, int, sizeof(int) * size,
						NULL, NULL);
	if (TOID_IS_NULL(array)) {
		fprintf(stderr, "POBJ_ALLOC\n");
		return OID_NULL;
	}

	for (size_t i = 0; i < size; i++)
		D_RW(array)[i] = (int)A[i];
	pmemobj_persist(pop, D_RW(array), size * sizeof(*D_RW(array)));
	return array.oid;
}



void AEMMergesort(TOID(int) A, int start, int end,int arraysize){//the mergesort that CMU proposed
    if(arraysize<=k*M){
        MergeSort(A,start,end);
    }
    int c=0;
    int  i;
    int round=0;
    lastV=INT32_MIN;
    int *buffer=new int [M];;
    int subarraysize=arraysize/L;
    for(i=0;i<L;i++){
        AEMMergesort(A,start+i*subarraysize,start+(i+1)*subarraysize-1,subarraysize);
    }
    while(c<arraysize){
        int k=0;
        for(i=0;i<L;i++)
            ProcessBlock(A,start,i,subarraysize);
        while(!Q.empty()){
            Element e=*Q.begin();
            Q.erase(e);
            buffer[k++]=e.value;
            c++;
            if(k==M){//flush the buffer into  the pmem
                for(int j=0;j<M;j++){
                    D_RW(PersistantO)[round*M+j]=buffer[j];
                }
                //pmemobj_memcpy_persist(pop,&O+round*M*sizeof(int),buffer,M);
                round++;
                lastV=buffer[M-1];
                k=0;
            }
            if(e.end){
                i=e.subarray;
                I[i]+=B;
                RestNum[i]=M;//the new block
                ProcessBlock(A,start,i,subarraysize);
            }
        }
        
    }
    for(i=0;i<arraysize;i++){
        int x=D_RO(PersistantO)[i];
        cout<<x<<endl;
    }
}

void Merge(TOID (int)A ,int p,int q,int r){//the normal mergesort in pmem
    TOID(int) L,R;
    int n1=q-p+1;
    int n2=r-q;

    POBJ_ALLOC(pop, &L, int, sizeof(int) * (n1+1),NULL, NULL);
	if (TOID_IS_NULL(L)) {
		fprintf(stderr, "POBJ_ALLOC\n");
	}
    POBJ_ALLOC(pop, &R, int, sizeof(int) * (n2+1),NULL, NULL);
	if (TOID_IS_NULL(R)) {
		fprintf(stderr, "POBJ_ALLOC\n");
	}
    int i,j;
    for(i=0;i<n1;i++)
        D_RW(L)[i]=D_RO(A)[p+i];
    for(j=0;j<n2;j++)
        D_RW(R)[j]=D_RO(A)[q+j+1];
    D_RW(L)[n1]=INT32_MAX;
    D_RW(R)[n2]=INT32_MAX;
    i=0;j=0;
    for(int k=p;k<=r;k++ ){
        if(D_RO(L)[i]<=D_RO(R)[j]){
            D_RW(A)[k]=D_RO(L)[i];
            i++;
        }
        else{
            D_RW(A)[k]=D_RO(R)[j];
            j++;
        }
    }
}

void MergeSort(TOID(int) A,int p,int r){
    if(p<r){
        int q=(p+r)/2;
        MergeSort(A,p,q);
        MergeSort(A,q+1,r);
        Merge(A,p,q,r);
    }
    
}

int main(int argc ,char *argv[]){
    int i;
    vector<int> A=ReadData("/home/chuzhaole/Desktop/sort/Data.txt");
    int subarraysize=A.size()/L;
    for(i=0;i*subarraysize<A.size();i++){//an array of pointers that point to the start of each sorted subarray
        I.push_back(i*subarraysize);
        RestNum.push_back(B);
    }
    const char *path = argv[1];
	pop = NULL;
    pop = pmemobj_create(path, "test",		PMEMOBJ_MIN_POOL, 0666);
    if (pop== NULL) {
			printf("failed to create pool\n");
			return 1;
		}
    PersistantA=alloc_int(A.size(),A);//put the array into pmem

	POBJ_ALLOC(pop, &PersistantO, int, sizeof(int) * A.size(),NULL, NULL);//alloc the output array
	if (TOID_IS_NULL(PersistantO)) {
		fprintf(stderr, "POBJ_ALLOC\n");
	}
    //AEMMergesort(A.size());
    MergeSort(PersistantA,0,A.size()-1);
    for(i=0;i<A.size();i++){
        int x=D_RO(PersistantA)[i];
        cout<<x<<endl;
    }
}