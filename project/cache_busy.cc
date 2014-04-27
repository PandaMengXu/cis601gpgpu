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

#define CACHE_LINE_SIZE 64


// ./a.out memsize(in KB)
int main(int argc, char** argv)
{
    if( argc < 4 ) {
        printf("[usage] ./program mem_size_KB iteration sleep_utime\n");
        exit(1);
    }

    unsigned long mem_size_KB = atol(argv[1]);  // mem size in KB
    unsigned long iteration = atol(argv[2]);
    unsigned long sleep_utime = atol(argv[3]);
    unsigned long mem_size_B  = mem_size_KB * 1024; // mem size in Byte
    unsigned long count       = mem_size_B / sizeof(long);
    unsigned long row         = mem_size_B / CACHE_LINE_SIZE;
    int           col         = CACHE_LINE_SIZE / sizeof(long);
    
    unsigned long long start, finish, dur1;
    unsigned long temp;

    if( iteration > row ){
        printf("Array row size is smaller than the iteration. Cannot record all data. exit\n");
        exit(1);
    }

    long *buffer;
    buffer = new long[count];

    // init array
    for(unsigned long i = 0; i < row; ++i)
        for( int j = 0; j < col; ++j)
                buffer[i*col+j] = i;

    // warm the cache again
    for(unsigned long i = 0; i < row; ++i) {
//            printf("%ld \t", i);
        for( int j = 0; j < col; ++j) {
            temp = buffer[i*col+j];
//            printf("%ld ", temp);
        }
//        printf("\n");
    }

    for( unsigned long k = 0; k < iteration; ++k ) {
        start = rdtsc();
        for( unsigned long i = 0; i < row; i+=1 ) {
            buffer[i*col] = buffer[i*col] * 2 - i; /*be careful not to write our time record*/
        }
        finish = rdtsc();
        //record teh duration in the second long of each cache line
        buffer[k*col+2] = finish - start;
        if( sleep_utime > 0 ) {
            usleep(sleep_utime);
        }
    }

    /* print out the time latency of each iteration */
    for ( unsigned long k = 0; k < iteration; ++k ){
        printf("%ld %ld\n", k, buffer[k*col+2]);
    }

    free(buffer);

    return 0;
}
