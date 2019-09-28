/*
 *   Copyright (C) 2019 Yongping Luo <ypluo18@qq.com>
*/

#include "Block.h"

uint64_t dreads = 0, preads = 0;
uint64_t dwrites = 0, pwrites = 0;

using std::cout;
using std::endl;

const int TEST_NUM = 200;
int main(void) {
    Array<TOID(int)>arr(TEST_NUM);

    cout << "==================end of the test==================" << endl;

    for(int i = 0; i < TEST_NUM; i++) {
        arr[i] = i * i;
    }

    for(int i = 0; i < TEST_NUM; i++) {
        uint64_t a = arr[i];
        cout << i << " " << a << endl;
    }

    cout << "DRAM Reads: " << dreads << " DRAM Writes: " << dwrites << endl;
    cout << "NVM Reads: " << preads << " NVM Writes: " << pwrites << endl;

    cout << "==================end of the test==================" << endl;
    return 0;
}