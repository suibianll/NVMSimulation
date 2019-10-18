/*
 *   Copyright (C) 2019 Yongping Luo <ypluo18@qq.com>
*/

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "param.h"
#include <iostream>
//#include <libpmemobj.h>

enum Place{DRAM = 0, NVM = 1};
const int BLOCK_SIZE = 16;


template<typename T>
class Ref {
    // This is a excellent method from user vczh and Zhang Jinwang on zhihu.com
    // when overload [] operator to Block, this class can be used to distinguish 
    // the reference to Block is read or write 
private:
    T * blk;
    int pos;
    Place ple;

public:
    Ref(T * b, int i, Place p):blk(b), pos(i), ple(p) { }
    Ref<T> & operator = (T v) { // write operation will meet = op
        if(ple == DRAM) {
            dwrites++;
            blk[pos] = v;
        } else {
            pwrites++;
            //TODO: NVM写操作， 写入一个T值
            blk[pos] = v;
            
        }
        return *this;
    }
    operator T() const { //read operation will be cast to T implicitly, met by T() op
        if(ple == DRAM) {
            dreads++;
            return blk[pos];
        } else {
            preads++;
            //TODO: NVM读操作, 返回一个T值
            return blk[pos];
        }
    }
};


template<typename T>
class Block {
private:
    Place placement;
    T * blk;

public:
    Block(Place p = DRAM):placement(p) {
        if(placement == DRAM) {
            blk = new T[BLOCK_SIZE];
        } else if (placement == NVM){
            //TODO: 调用NVM分配接口分配内存空间
            blk = new T[BLOCK_SIZE];
        } else {
            std::cout << " Bad Placement == " << placement << std::endl;
            exit(-1);
        }
    }

    ~Block() {
        if(placement == DRAM) {
            delete [] blk;
        } else {
            //TODO: 调用NVM接口释放内存空间
            delete [] blk;
        }
    }

    Ref<T> operator [](int idx) { // this place need to distinguish read and write operation
        return Ref<T>(blk, idx, placement);
    }
};

template<typename T>
class Array {
private:
    const double RATIO = 1.0 / 11;
    Block<T> * dram_blks = NULL;
    Block<T> * nvm_blks = NULL;
    uint32_t ttl_num = 0;
    uint32_t blk_num = 0;
    uint32_t dram_num = 0;
    uint32_t nvm_num = 0;

public:
    Array(uint32_t n):ttl_num(n){

        blk_num = ttl_num / BLOCK_SIZE + 1;

        dram_num = int(RATIO * blk_num);

        dram_blks = reinterpret_cast<Block<T> *>(new char[sizeof(Block<T>) * dram_num]);
        for(int i = 0; i < dram_num; i++) {
            new (dram_blks + i) Block<T>(DRAM);
        }

        nvm_num = blk_num - dram_num;

        nvm_blks = reinterpret_cast<Block<T> *>(new char[sizeof(Block<T>) * nvm_num]);
        for(int i = 0; i < nvm_num; i++) {
            new (nvm_blks + i) Block<T>(NVM);
        }
    }

    ~Array() {
        delete [] dram_blks;
        delete [] nvm_blks;
    }

    Ref<T> operator[] (int idx) {
        if(idx >= ttl_num || idx < 0){ // border check
            std::cout << " out of array border " << std::endl;
            exit(-1);
        }

        int blk_no = idx / BLOCK_SIZE;

        if(blk_no < dram_num) {
            return dram_blks[blk_no][idx % BLOCK_SIZE];
        } else {
            return nvm_blks[blk_no - dram_num][idx % BLOCK_SIZE];
        }
    }
};

#endif // _BLOCK_H_