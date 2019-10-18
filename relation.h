#ifndef __RELATION_H__
#define __RELATION_H__

#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cinttypes>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>

//file io in linux system
#ifdef __linux__
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

typedef struct timeval TC_t;
#define TC_START(start) gettimeofday(&start,NULL)
#define TC_END(End) gettimeofday(&end,NULL)
#define TC_CALC(start, end) (end.tv_sec - start.tv_sec + (double)(end.tv_usec - start.tv_usec)/1000000)

#endif

#include "param.h"

typedef uint32_t _key_t;
typedef uint32_t _value_t;
typedef uint8_t Byte;

struct Record {
    _key_t key;
    _value_t value;
};

inline uint32_t modulos(const Record & r, int hashspace) {
    return r.key % hashspace;
}

inline uint32_t modulos2(const Record & r, int hashspace) {
    return (r.key + hashspace / 2) % hashspace;
}
uint32_t murmur(Record &r, int hash_space);
inline uint32_t partition(const Record & r, int partition_cnt)
{
    return (r.key>>10) % partition_cnt;
}

void my_sort(Record * R, int size, float read_dly=0, float write_dly=0);
void my_sort(std::vector<Record> & R, int size, float read_dly=0, float write_dly=0);
void my_sort_ADEC(Record *R, int size, float read_dly=0, float write_dly=0);

void emulate_latency_ns(float ns);
int get_file_size(int);

enum ResidePos{dram = 0, nvm = 1};
extern float readDelays[2];
extern float writeDelays[2];

class Residence {
public:
    ResidePos pos;
    Residence(ResidePos rsdPos) : pos(rsdPos) {}
    inline void change_residence(ResidePos rsdPos) {
        pos = rsdPos;
    }
	inline void read_delay(size_t sz) const {
        if(readDelays[pos] > 0)
            emulate_latency_ns(readDelays[pos] * sz);
    }
    inline void write_delay(size_t sz) const {
        if(writeDelays[pos] > 0)
            emulate_latency_ns(writeDelays[pos] * sz);
    }
};
#endif //__RELATION_H__
