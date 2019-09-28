#include <iostream>
#include <vector>
#include<fstream>
#include<time.h>
#include<cstdio>
using namespace std;

#define Size 10000

vector<long> Wtime;
vector<long> Rtime;

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

vector<int> GenData(){
    srand((int) time(0));
    vector<int> result;
    for (int i=0;i<Size;i++){
        result.push_back(rand()%Size*10);
    }
    return result;
}

void HeapAdjust(vector<int> &list, int parent, int length){
	int temp = list[parent];					// temp保存当前父节点
	int child = 2 * parent + 1;					// 先获得左孩子
    Rtime[parent]++;
	while (child < length){
		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (child + 1 < length && list[child] < list[child + 1]){
			child++;
            Rtime[child]++;
            Rtime[child+1]++;
		}

		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (temp >= list[child]){
            Rtime[child]++;
			break;
		}

		// 把孩子结点的值赋给父结点
		list[parent] = list[child];
        Wtime[parent]++;
		Rtime[child]++;
		// 选取孩子结点的左孩子结点,继续向下筛选
		parent = child;
		child = 2 * parent + 1;
	}
	list[parent] = temp;
    Wtime[parent]++;
}

vector<int> HeadSort(vector<int> list){
	int length = list.size();
	// 循环建立初始堆
	for (int i = length / 2 - 1; i >= 0; i--){
		HeapAdjust(list, i, length);
	}

	// 进行n-1次循环，完成排序
	for (int i = length - 1; i > 0; i--){
		// 最后一个元素和第一元素进行交换
		int temp = list[i];
		list[i] = list[0];
		list[0] = temp;
		Wtime[0]++;
		Rtime[0]++;
		Wtime[i]++;
		Rtime[i]++;
		// 筛选 R[0] 结点，得到i-1个结点的堆
		HeapAdjust(list, 0, i);
		/*cout << "第" << length - i << "趟排序:";
		for (int i = 0; i < list.size(); i++){
			cout << list[i] << " ";
		}
		cout << endl;*/
	}
	return list;
}

int main(){
    vector<int> test=GenData();
    vector<long> Wtime1(test.size());
	vector<long>Rtime1(test.size());
	vector<double> RatW(test.size());
	vector<double> RatR(test.size());
    Wtime=Wtime1;
	Rtime=Rtime1;
	vector<int> result;
	result = HeadSort(test);
    FILE *OutfpW;
    OutfpW=fopen("/home/chuzhaole/Desktop/sort/heapsortW.txt","w");
	FILE *OutfpR;
    OutfpR=fopen("/home/chuzhaole/Desktop/sort/heapsortR.txt","w");
	cout << "排序后:";
	for (int i = 0; i < result.size(); i++){
		fprintf(OutfpW,"%d,%ld\n",i,Wtime[i]);
		fprintf(OutfpR,"%d,%ld\n",i,Rtime[i]);
		//cout <<i<<"  "<< result[i] << " "<<Wtime[i]<<endl;
		/*if(i>0){
			Wtime[i]+=Wtime[i-1];
			Rtime[i]+=Rtime[i-1];
		}*/
			
	}
	/*for (int i = 0; i < result.size(); i++){
			RatW[i]=double(Wtime[i])/Wtime[result.size()-1];
			RatR[i]=double(Rtime[i])/Rtime[result.size()-1];
			if(i%1000==0){
				fprintf(OutfpW,"%.4f,%f\n",double(i)/Size,RatW[i]);
				fprintf(OutfpR,"%.4f,%f\n",double(i)/Size,RatR[i]);
			}
			
	}*/
	printf("%ld",Wtime[result.size()-1]);

    return 0;
}