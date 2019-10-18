all: main.cpp SortMerge.h Relation.h Heap.h  
	g++ -std=c++11 main.cpp -g -o main -lm

debug: main.cpp SortMerge.h Relation.h Heap.h  
	g++ -std=c++11 -g main.cpp -o main -lm

clean :
	rm -r *.o *.exe
