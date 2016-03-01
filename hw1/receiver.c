#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <inttypes.h>

#define ARRAY_LENGTH    (4096+8)
#define ARRAY_EFFECTIVE_LENGTH  4096

#define SET_NUM         (64)
#define LINE_NUM        (8)
#define ELMT_PER_LINE   (8)

#define dprintf(fmt, arg...)	printf( fmt, ## arg )
#define dfprintf(file, fmt, arg...)	fprintf( file, fmt, ## arg)

float miss_hit_ratio_manual = 2.2;

static inline long get_elapsed_usec(struct timeval before, struct timeval after)
{
    //dprintf("get_elapsed_usec: after.tv_sec = %ld, after.tv_usec = %ld; before.tv_sec=%ld, before.tv_usec=%ld\n", after.tv_sec, after.tv_usec, before.tv_sec, before.tv_usec);
    return ( (after.tv_sec - before.tv_sec) * 1000000 ) + ( after.tv_usec - before.tv_usec );
}

static inline int get_array_index(int set, int line)
{
    return set * ELMT_PER_LINE + line * SET_NUM * ELMT_PER_LINE;
}

void main()
{
    unsigned long* array = NULL;
    int set = 0, line = 0, k = 0;
    int start = 0, end = 0;
    struct timeval before, after;    
    int bad = 0, try = 0, i = 0, tmp = 0;
    int bit = 0;
    float miss_time_all, miss_time, hit_time_all, hit_time, miss_hit_ratio, miss_hit_diff;
    uint32_t buffer = 0;

    /* Test: unsigned long must be 64bit*/
    if( sizeof(unsigned long) != 8)
    {
        dfprintf(stderr, "Error: unsigned long must be 8 Byte for the program to work\n");
        exit(1);
    }
    
    /* init array and load L1 data cache*/
    do
    {
        array = malloc(sizeof(unsigned long) * ARRAY_LENGTH);
    }while( array == NULL && try++ <= 10);
    if( array == NULL)
    {
        dfprintf(stderr, "Error: cannot allocate array size:%d\n", ARRAY_LENGTH);
        exit(2);
    }

    /* Allign the start of the array to be the start of a cache line*/
    if( ( (unsigned long)array & 0x038UL ) != 0) /*Start of array is not start of a cache line*/
    {
        start = (0x08 - (( (unsigned long)array & 0x038UL ) >> 0x3));
    }else{
        start = 0;
    }
    dprintf("Allign the array with the cache line: start element's index is:%d\n", start);
    
    /* Initialize the array, load all L1 data cache, first round is cache miss, second round is cache hit, get ratio of cache miss/cach hit*/
    for(i = 0; i < 2; i++)
    {
        for(set = 0; set < SET_NUM; set+=2)
        {
            bad = gettimeofday(&before, NULL);
            assert( !bad );
            for( line = 0; line < LINE_NUM; line++)
            {
                for( k = get_array_index(set, line); 
                        k < get_array_index(set, line) + ELMT_PER_LINE; k++)
                {   
                    if(i == 0)
                        array[k] = 0xFFFFFFFFFFFFFFFF;
                    else
                        tmp = array[k];
                }
            }
            bad = gettimeofday(&after, NULL);
            assert( !bad );
            if( i == 0 )
                miss_time_all += get_elapsed_usec(before, after);
            else
                hit_time_all += get_elapsed_usec(before, after);
        }
        if( i == 0 )
        {
            miss_time = miss_time_all / (SET_NUM / 2);
        }
        else
        {
            hit_time = hit_time_all / (SET_NUM / 2);
        }

    }
    dprintf("Initialize the L1D array: Each set(miss) takes:%.2fus; All array(miss) takes %.2fus in total\n", miss_time, miss_time_all);
    dprintf("Reload the L1D array: Each set(hit) takes:%.2fus; All array(hit) takes %.2f in total\n", hit_time, hit_time_all);
    miss_hit_ratio = miss_time * 1.0 / hit_time;
    miss_hit_diff = miss_time - hit_time;
    dprintf("Ratio of miss to hit per line is %.2f\n", miss_hit_ratio);
      
    sleep(2);
    /* Start the covert channel*/
    dprintf("Start receiving the data\n");
    buffer = 0;
    for(set = 0; set < SET_NUM; set += 2) /* Avoid prefetch effect*/
    {
        bit = 0;
        bad = gettimeofday(&before, NULL);
        assert( !bad );
        for( line = 0; line < LINE_NUM; line++)
        {
            for( k = get_array_index(set, line);
                    k < get_array_index(set, line) + ELMT_PER_LINE; k++)
            {
                tmp = array[k]; 
            }
        }
        bad = gettimeofday(&after, NULL);
        assert( !bad );
        //if(get_elapsed_usec(before, after) / hit_time >= (miss_hit_ratio*3.0/5)) /*miss means the bit is 1*/
	if(get_elapsed_usec(before, after) - hit_time >= (miss_hit_diff * 0.5) )
        {
            bit = 1;
        }
	dprintf("set %d (bit %d): latency is %ld, miss_hit_diff is %.2f, bit received is: %d\n", set, set/2, get_elapsed_usec(before, after), get_elapsed_usec(before, after) - hit_time, bit);

	buffer |= (bit << set);
        //printf("Reload time is %ld, hit_time is %.2f, miss_hit_ratio is %.2f\n", get_elapsed_usec(before, after), hit_time, get_elapsed_usec(before, after) / hit_time);
/*        if( set%8 == 0 )
            printf(" ");
        printf("%d", bit);
*/
    }
/*    printf("\n"); */
    printf("Receive 32bit: %#010x\n", buffer);
    
    free(array);
}
