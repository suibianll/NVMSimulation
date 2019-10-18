// characteristic of disk
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
#define CPUFREQ 2000  //Cpu frequency: unit:MHZ
#define READ_DELAY (10.0 / CACHE_LINE_SIZE) // extra read delay: 10ns per CL
#define WRITE_DELAY (200.0 / CACHE_LINE_SIZE) // extra write delay: 200ns per CL
