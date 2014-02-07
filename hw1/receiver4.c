#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <inttypes.h>

#define ARRAY_LENGTH    (4096 * 2)
#define CACHE_ARRAY_SIZE 	(4096*8)
//#define ARRAY_EFFECTIVE_LENGTH  4096

#define SET_NUM         (64)
#define LINE_NUM        (8)
#define ELMT_PER_LINE   (8)

#define SET_STEP	1

#define dprintf(fmt, arg...)	printf( fmt, ## arg )
#define dfprintf(file, fmt, arg...)	fprintf( file, fmt, ## arg)

unsigned hi, lo;

/*static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}*/
#define rdtsc(void)\
({\
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));\
    ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );\
})


static inline float get_elapsed_usec(struct timeval before, struct timeval after)
{
    //dprintf("get_elapsed_usec: after.tv_sec = %ld, after.tv_usec = %ld; before.tv_sec=%ld, before.tv_usec=%ld\n", after.tv_sec, after.tv_usec, before.tv_sec, before.tv_usec);
    return ( ( (after.tv_sec - before.tv_sec) * 1000000.0 ) + 1.0 * ( after.tv_usec - before.tv_usec ) ); 
}


static inline int get_array_index(int set, int line)
{
    return set * ELMT_PER_LINE + line * SET_NUM * ELMT_PER_LINE;
}

static inline void enumerate_array(char* array)
{
    int i = 0, j; 
    unsigned long t_start, t_end;
    for( i = 0; i < CACHE_ARRAY_SIZE; i+=64)
//    for( j = 0; j < CACHE_ARRAY_SIZE; j+=1)
    {
        t_start = rdtsc();
        array[i] = i;
	t_end = rdtsc();
	printf("access address %#lx, latency = %lu\n",(unsigned long) (array+i), t_end-t_start);
    }
    return;
}


void main()
{
//    unsigned long* array = NULL, *array_0;
    char array[CACHE_ARRAY_SIZE];
    int set = 0, line = 0, k = 0;
    int start = 0, end = 0;
    struct timeval before, after;    
    int bad = 0, try = 0, i = 0, tmp = 0;
    int bit = 0;
    float miss_time_all, miss_time, hit_time_all, hit_time, miss_hit_ratio, miss_hit_diff,miss_hit_threshhold, time_elapsed;
    uint32_t buffer = 0;
    unsigned long temp;
    unsigned long t_start, t_end;

    /* Initialize the array, load all L1 data cache, first round is cache miss, second round is cache hit, get ratio of cache miss/cach hit*/
    for(i = 0; i < 2; i++)
    {
 //       bad = gettimeofday(&before, NULL);
 //       assert( !bad );
	t_start = rdtsc();
	enumerate_array(array);
/*        for(set = 0; set < SET_NUM; set+=SET_STEP)
        {
            for( line = 0; line < LINE_NUM; line++)
            {
                k = get_array_index(set, line);
                array[k] = set  +  line;
		temp = array[k] + set;
            }
        }
*/
//        bad = gettimeofday(&after, NULL);
//        assert( !bad );
	t_end = rdtsc();

        if( i == 0 )
        {
//            miss_time_all = get_elapsed_usec(before, after);
	    miss_time_all = t_end - t_start;
        }
        else
        {
//            hit_time_all = get_elapsed_usec(before, after);
	    hit_time_all = t_end - t_start;
        }

    }
    miss_hit_diff = miss_time_all - hit_time_all;
    miss_hit_threshhold = 400;
    dprintf("Initialize the L1D array: All array(store miss) takes %.2fus in total\n",  miss_time_all);
    dprintf("Restore the L1D array: All array(store hit) takes %.2f in total\n", hit_time_all);
    dprintf("miss_hit_diff = %.2f; miss_hit_threshhold = %.2f\n", miss_hit_diff, miss_hit_threshhold);
      
    sleep(2);
    /* Start the covert channel*/
    dprintf("Start receiving the data\n");
    while(1)
    {
        time_elapsed = 0;
//        bad = gettimeofday(&before, NULL);
//        assert( !bad );
	t_start = rdtsc();
        enumerate_array(array);
/*        for(set = 0; set < SET_NUM; set += SET_STEP) 
        {
            bit = 0;
            for( line = 0; line < LINE_NUM; line++)
            {
		k = get_array_index(set, line);
                array[k] = set +  line;
		temp =  array[k] + set;
            }
        }
*/
//        bad = gettimeofday(&after, NULL);
//        assert( !bad );
	t_end = rdtsc();
//        time_elapsed = get_elapsed_usec(before, after);
	time_elapsed = t_end - t_start;
        if( time_elapsed - hit_time_all >= miss_hit_threshhold )
        {
            bit = 1;
        }else
	{
	    bit = 0;
	}
        dprintf("bit recieved is %d : latency is %.2f, diff is %.2f,  miss_hit_threshold is %.2f\n", bit, time_elapsed, time_elapsed - hit_time_all, miss_hit_threshhold);
        printf("%d\n", bit);
	sleep(1);
	sleep(1);
    }

    free(array);
}
