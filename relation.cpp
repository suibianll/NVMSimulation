#include "relation.h"
#include <algorithm>

bool (*cmp)(const Record &, const Record &);

uint32_t murmur(Record &r, int hash_space) {
	uint32_t h = r.key;
	h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h % hash_space;
}

inline bool my_less(const Record & a, const Record & b) 
{
    return a.key < b.key;
}

inline bool my_greater(const Record &a, const Record & b){
    return a.key > b.key;
}

template<class Records>
int random_part(Records & recs, int s, int e, int * reads, int *writes) {
    // randomly choose a pivot
    int rand_id = (rand() % (e - s + 1)) + s;
    //emulate_latency_ns(2 * delay_ns);
    *writes += 2;
    *reads += 2;
    Record pivot = recs[rand_id];
    recs[rand_id] = recs[e];
    recs[e] = pivot;
    // start partition
    int j = s - 1;
    for(int i = s; i <= e - 1; i++) {
        (*reads)++;
        if(cmp(recs[i], pivot)) {
            j += 1;
            //emulate_latency_ns(2 * delay_ns);
            *writes += 2;
            *reads += 2;
            Record tmp = recs[j];
            recs[j] = recs[i];
            recs[i] = tmp;
        }
    }
    //emulate_latency_ns(2 * delay_ns);
    *writes += 2;
    *reads += 2;
    recs[e] = recs[j + 1];
    recs[j + 1] = pivot;
    return j + 1; // return the position that the pivot should be
}

template<class Records>
void my_quicksort(Records & recs, int start, int end, int * reads, int *writes){
    if (start < end) {
        int i = random_part(recs, start, end, reads, writes);
        my_quicksort(recs, start, i - 1, reads, writes);
        my_quicksort(recs, i + 1, end, reads, writes);
    }
}

void my_sort(Record * R, int size, float read_dly, float write_dly) {
    cmp = my_less;
    int writes = 0;
    int reads = 0;
    my_quicksort(R, 0, size - 1, &reads, &writes);
    emulate_latency_ns(read_dly * reads + write_dly * writes);
}

void my_sort(std::vector<Record> & R, int size, float read_dly, float write_dly) {
    cmp = my_less;
    int writes = 0;
    int reads = 0;
    my_quicksort(R, 0, R.size() - 1, &reads, &writes);
    emulate_latency_ns(read_dly * reads + write_dly * writes);
}

void my_sort_ADEC(Record * R, int size, float read_dly, float write_dly) {
    cmp = my_greater;
    int writes = 0;
    int reads = 0;
    my_quicksort(R, 0, size - 1, &reads, &writes);
    emulate_latency_ns(read_dly * reads + write_dly * writes);
}

#ifdef __linux__
int get_file_size(int fd) {
    struct stat buf;
    fstat(fd, &buf);
    return ceil((double)buf.st_size / PAGE);
}
#endif

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

void emulate_latency_ns2(float ns)
{
	int cycles = (ns * CPUFREQ) / 1000;

	for(int i = 0; i < cycles; i+=10) {
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
    }
}
