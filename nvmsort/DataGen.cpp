#include <iostream>
#include <vector>
#include <time.h>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;

void HeapAdjust(vector<int> &list, int parent, int length){
	int temp = list[parent];					// temp保存当前父节点
	int child = 4 * parent + 1;					// 先获得左孩子
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
		child = 4 * parent + 1;
	}
	list[parent] = temp;
}

int main(int argc, char *argv[]){
    if(argc<2){
        cout<<"you should give the size  and the type of this dataset"<<endl;
    }
    int size=atoi(argv[1]);
    string ss=to_string(size);
    
    //char *model=argv[2];
    FILE *Outfp;
    string filepath1="Randomdata"+ss+".txt";
    Outfp=fopen(filepath1.c_str(),"w");
    FILE *OutfpP;
    string filepath2="PartSortdata"+ss+".txt";
    OutfpP=fopen(filepath2.c_str(),"w");
    srand(int(time(0)));
    int i,temp;
    vector<int> data;
    for(i=0;i<size;i++){
            temp=rand()%(size*10);
             data.push_back(temp);
            fprintf(Outfp,"%d\n",temp);
    }
    for(i=size/2-1;i>=0;i--)
        HeapAdjust(data,i,size);
    for(i=0;i<size;i++)
        fprintf(OutfpP,"%d\n",data[i]);
    

}