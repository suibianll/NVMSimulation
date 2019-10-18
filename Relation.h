/*
 *   Copyright (C) 2019 Yongping Luo <ypluo18@qq.com>
*/

#ifndef _RELATION_H_
#define _RELATION_H_
//#include "param.h"
#include <iostream>
#include <math.h>
 #include <unistd.h> 
 #include <fcntl.h>
#include <stdint.h> 
//#include <libpmemobj.h>
#include <set>
#include <cstdlib>
#include <ctime>

#define CPUFREQ 2000  //Cpu frequency: unit:MHZ
#define delay 40

#define PAGE 4096 // page size : byte
#define PAGE_PER_BLOCK 16
#define BLKSIZE (PAGE_PER_BLOCK * PAGE)

#define KILLION 1024
#define MILLION (KILLION * KILLION)

#define ISIZE 256 // unit:page
#define MSIZE 1024 //4MB
#define PSIZE 2048 //8MB
#define RSIZE 4096 //16MB
#define SSIZE 16384 //64MB

#define CACHE_LINE_SIZE 64
//#define CPUFREQ 2000  //Cpu frequency: unit:MHZ
#define READ_DELAY (10.0 / CACHE_LINE_SIZE) // extra read delay: 10ns per CL
#define WRITE_DELAY (200.0 / CACHE_LINE_SIZE) // extra write delay: 200ns per CL
 uint64_t preads, dreads;
 uint64_t pwrites, dwrites;
/*POBJ_LAYOUT_BEGIN(array);
POBJ_LAYOUT_TOID(array, int);
POBJ_LAYOUT_TOID(array, PMEMoid);
POBJ_LAYOUT_END(array);
static PMEMobjpool *pop;

inline int pmem_init(std::string path) {
    std::string file = path + "/test";
    if( (access( file.c_str(),  F_OK)) != -1 )  {
        remove(file.c_str());
    }
    pop = pmemobj_create(file.c_str(), "test", PMEMOBJ_MIN_POOL, 0666);
    if (pop== NULL) {
		std::cout << "failed to create pool" << std::endl;
		return -1;
	}
    return 0;
}*/
static inline unsigned long long asm_rdtsc(void)
{
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

void emulate_latency_ns(float ns)
{
    unsigned long long cycles = 0;
	unsigned long long start = 0;
	unsigned long long stop = 0;
	
	start = asm_rdtsc();
	cycles = (unsigned long long)(ns * CPUFREQ) / 1000;

	do { 
		stop = asm_rdtsc();
	} while (stop - start < cycles);
}
enum Place{DRAM = 0, NVM = 1};
const int BLOCK_SIZE = 16;
typedef int32_t _key_t;
typedef int32_t _value_t;
typedef uint8_t Byte;
struct Record {
    _key_t key;
    _value_t value;
};

class Ref {
    // This is a excellent method from user vczh and Zhang Jinwang on zhihu.com
    // when overload [] operator to Block, this class can be used to distinguish 
    // the reference to Block is read or write 
private:
    Record * blk;
    Record *  pblk;
    int pos;
    Place ple;

public:
    Ref(Record * b, Record * pb, int i, Place p):blk(b),pos(i), ple(p) { 
        pblk = pb;
    }
    Ref & operator = (Record v) { // write operation will meet = op
        if(ple == DRAM) {
            dwrites++;
            blk[pos] = v;
        } else {
            pwrites++;
            //usleep(0.001);
            emulate_latency_ns(delay);
            pblk[pos] =v;
        }
        return *this;
    }



    Ref & operator = (Ref & r) { // write operation will meet = op
        if(ple == DRAM) {
            dwrites++;
            blk[pos] = Record(r);
        } else {
            pwrites++;
            //usleep(0.001);
            emulate_latency_ns(delay);
            pblk[pos] =Record(r);
        }
        return *this;
    }
    int key(){
        if(ple==DRAM){
            dreads++;
            return blk[pos].key;
        }
        else{
            preads++;
            return pblk[pos].key;
        }
    }

    int value(){
        if(ple==DRAM){
            dreads++;
            return blk[pos].value;
        }
        else{
            preads++;
            return pblk[pos].value;
        }
    }

    operator Record() const { //read operation will be cast to int implicitly, met by Record() op
        if(ple == DRAM) {
            dreads++;
            return blk[pos];
        } else {
            preads++;
            return (pblk[pos]);
        }
    }
};


class Block {
private:
    Place placement;
    Record * blk;
    Record *  pblk;

public:
    Block(Place p = DRAM):placement(p) {
        if(placement == DRAM) {
            blk = new Record[BLOCK_SIZE];
        } else if (placement == NVM){
            blk = NULL;
            //POBJ_ALLOC(pop, &pblk, int, sizeof(int) *BLOCK_SIZE, NULL, NULL);
            pblk = new Record[BLOCK_SIZE];
        } else {
            std::cout << " Bad Placement == " << placement << std::endl;
            exit(-1);
        }
    }

    ~Block() {
        if(placement == DRAM) {
            delete [] blk;
        } else {
            //POBJ_FREE(&pblk);
            delete [] pblk;
        }
    }

    Ref operator [](int idx) { // this place need to distinguish read and write operation
        return Ref(blk, pblk, idx, placement);
    }
};


enum  Pattern{PURE_DRAM = 0, PURE_NVM, HYBRID_SEG, HYBRID_RAND};

const double DEFAULT_RATIO = 1.0 / 11;
class Relation {
private: 
    Block * blks;
    uint32_t arr_size = 0;
    uint32_t space_size=0;
    uint32_t blk_num = 0;
    double ratio;
    bool heapdeleted=false;

public:
    Relation(uint32_t n, double r = DEFAULT_RATIO, Pattern p = HYBRID_RAND): space_size(n), ratio(r){

        blk_num = ceil(double(space_size) / BLOCK_SIZE);

        // allocate memory space
        blks = reinterpret_cast<Block *>(new char[sizeof(Block) * blk_num]);
        uint32_t dram_num ;
        std::set<int> tmp_set;
         int j = 0;
        switch(p) {
            case PURE_DRAM:
                for(int i = 0; i < blk_num; i++) {
                    new (blks + i) Block(DRAM);
                }
                break;

            case PURE_NVM:
                for(int i = 0; i < blk_num; i++) {
                    new (blks + i) Block(NVM);
                }
                break;

            case HYBRID_SEG:
                dram_num = int(ratio * blk_num);

                for(int i = 0; i < dram_num; i++) {
                    new (blks + i) Block(DRAM);
                }

                for(int i = dram_num; i < blk_num; i++) {
                    new (blks + i) Block(NVM);
                }
                break;

            case HYBRID_RAND: // distributed randonly in the Relation
                dram_num = int(ratio * blk_num);
                srand(time(NULL));
                
                while(tmp_set.size() < dram_num) {
                    tmp_set.insert(rand() % blk_num);
                }
                
               
                for(int i = 0; i < blk_num; i++) {
                    if(tmp_set.find(i) != tmp_set.end()) { // find i in the set
                        new (blks + i) Block(DRAM);
                        j++;
                    }
                    else {
                        new (blks + i) Block(NVM);
                    }
                }

                //std::cout << tmp_set.size() << " j " << j << std::endl;
                tmp_set.clear();
                break;

            default: 
                break;
        }
    }

    ~Relation() {
            for(int i = 0; i < blk_num; i++) {
            blks[i].~Block();
           
        }
    }
    uint32_t Size(){
        return arr_size;
    }

    uint32_t SpaceSize(){
        return space_size;
    }
    void SizeMinus(int x){
        arr_size-=x;
    }

    void SizePlus(int x){
        arr_size+=x;
    }

    Ref operator[] (int idx) {
        if(idx >= space_size || idx < 0){ // border check
            int i=0;
            std::cout << " out of Relation border " << std::endl;
            exit(-1);

        }
        return blks[idx / BLOCK_SIZE][idx % BLOCK_SIZE];
    }
};

#endif // _BLOCK_H_