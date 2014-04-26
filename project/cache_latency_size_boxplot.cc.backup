#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

using namespace std;

#if defined(__i386__)
static __inline__ unsigned long long rdtsc(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}
#elif defined(__x86_64__)
static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}
#endif

#define CACHE_LINE_SIZE	64

#define WSS 24567 /* 24 Mb */
#define NUM_VARS WSS * 1024 / sizeof(long)

// ./a.out memsize(in KB)
int main(int argc, char** argv)
{
	unsigned long mem_size_KB = atol(argv[1]);  // mem size in KB
	unsigned long mem_size_B  = mem_size_KB * 1024;	// mem size in Byte
    unsigned long count       = mem_size_B / sizeof(long);
    unsigned long row         = mem_size_B / CACHE_LINE_SIZE;
    int           col         = CACHE_LINE_SIZE / sizeof(long);
    
    unsigned long long start, finish, dur1;
    unsigned long temp;

    long *buffer;
    buffer = new long[count];

    // init array
    for (unsigned long i = 0; i < count; ++i)
        buffer[i] = i;

    for (unsigned long i = row-1; i >0; --i) {
        temp = rand()%i;
        swap(buffer[i*col], buffer[temp*col]);
    }

    // warm the cache again
    temp = buffer[0];
    for (unsigned long i = 0; i < row-1; ++i) {
        temp = buffer[temp];
    }

    // should be cache hit
    temp = buffer[0];
    start = rdtsc();
    int sum = 0;
    for (unsigned long i = 0; i < row-1; ++i) {
        if (i%2 == 0) sum += buffer[temp];
        else sum -= buffer[temp];
        temp = buffer[temp];
    }
    finish = rdtsc();
    dur1 = finish-start;

    // Res
	printf("%lld %lld\n", dur1, dur1/row);

    free(buffer);

	return 0;
}
