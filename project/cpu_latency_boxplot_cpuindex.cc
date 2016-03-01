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

#define WSS 24567 /* 24 Mb */
#define NUM_VARS WSS * 1024 / sizeof(long)

// ./a.out memsize(in KB)
int main(int argc, char** argv)
{
    
    unsigned long long iteration;
    unsigned long sleep_time;
    unsigned long long start, finish, dur1;
    unsigned long sum;
    cpu_set_t mask;
    int NUM_PROCS;
    int cpu_index;

    if(argc < 3) {
        printf("[usage]./cpu_latency_boxplot iternation sleep_time_us\n");
        exit(1);
    }
    
    iteration = atol(argv[1]);
    sleep_time = atol(argv[2]);
    cpu_index = atoi(argv[3]);

    //number of cores available for guest domUs
    NUM_PROCS = sysconf(_SC_NPROCESSORS_CONF);
    if( cpu_index >= NUM_PROCS)
        printf("cpu_index > NUM_PROCS\n");
    
    for(unsigned long j = 0; j < iteration; ++j) {
        sum = 0;
        
        CPU_ZERO( &mask );
        cpu_index = cpu_index % (NUM_PROCS);
        CPU_SET( cpu_index, &mask ); /*cpu 0 is for dom0*/
        /* set afficnity */
        if( sched_setaffinity( 0, sizeof(mask), &mask ) == -1 ) {
              printf("WARNING: Could not set CPU Affinity, continuing...\n");
        }

        start = rdtsc();
        for (unsigned long i = 0; i < 200000000; ++i) {
            sum = i*i + 1;
	    }
        finish = rdtsc();
        dur1 = finish-start;
        // Res
        printf("%ld %d %lld\n", j, cpu_index, dur1); 
	    
        if(sleep_time > 0) {
            usleep(sleep_time);
        }
    }

    return 0;
}
