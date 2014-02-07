#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <inttypes.h>

#define ARRAY_LENGTH            (4096*2)
#define CACHE_ARRAY_SIZE	4096

#define SET_NUM     (64)
#define LINE_NUM    (8)
#define ELMT_PER_LINE   (8)

#define SET_STEP  	1

unsigned long* array = NULL;
unsigned long* array_0 = NULL;

static inline int get_array_index(int set, int line)
{
    return set * ELMT_PER_LINE + line * SET_NUM * ELMT_PER_LINE;
}

static inline void enumerate_array(unsigned long* array)
{
    int i;
    for( i = 0; i < CACHE_ARRAY_SIZE; i+=8)
    {
        array[i] = i * 2;
    }
}


static inline void send_bit_1( )
{
    int line = 0, k = 0, i, j;
    int set = 0;
    int temp;
    for(j = 0; j < 2; j++)
    {
/*	for( i = 0; i<ARRAY_LENGTH; i++)
	{
		array[i] = i;
		temp = array[i] + ARRAY_LENGTH - i;
	}
*/	
/*       for(set = 0; set < SET_NUM; set+=SET_STEP)
        {
            for( line = 0; line < LINE_NUM; line++)
            {
                k = get_array_index(set, line); 
                array[k] = SET_NUM - set; 
                temp = array[k] + line;
            }
        }
*/
    }
    enumerate_array(array);
    printf("Just set bit 1 by pollute the cache");
}

/*send_bit_0: empty function for ease of read*/
static inline void send_bit_0(int bit_index, unsigned long* array)
{
}

static inline char get_rand_char()
{
    srand(time(NULL));
    return (char) rand() & 0x0FF;
}

void main()
{
    int set = 0, line = 0, k = 0;
    int start = 0, end = 0;
    int try = 0, i = 0, j = 0;
    int bit = 0;
    char buffer = 0;
    int send_times = 1000;
    
    
    /* Test: unsigned long must be 64bit*/
    if( sizeof(unsigned long) != 8 || sizeof(unsigned int) != 4)
    {
        fprintf(stderr, "Error: unsigned long must be 8 Byte and unsigned int must be 4 Byte for the program to work\n");
        exit(1);
    }

    /* allocate array and load L1 data cache*/
    do
    {
        array_0 = malloc(sizeof(unsigned long) * ARRAY_LENGTH);
        printf("Malloc the array\n");
    }while( array_0 == NULL && try++ <= 10);
    if( array_0 == NULL)
    {
        fprintf(stderr, "Error: cannot allocate array size:%d\n", ARRAY_LENGTH);
        exit(2);
    }

    /* Allign the start of the array to be the start of a cache line*/
    if( ( (unsigned long)array_0 & 0x0FFFUL ) != 0) /*Start of array is not start of a cache line*/
    {
        array = (unsigned long *) ((char*)array_0 + ( 0x0FFFUL - ((unsigned long)array_0 & 0x0FFFUL) ) + 1);
    }else{
        array = array_0;
    }
    printf("Array starting address is alligned; array=%#010lx\n", (unsigned long)array);


   // printf("Sender send %d bits data:%#010x\n", BITS_WIDTH, buffer);
    
    sleep(1);
    for( j = 0; j < send_times; j++)
    {
        /* Generate the bits to send*/
        buffer =  get_rand_char();
        for( i = 0; i < 8; i++ )
        {
            if( ( buffer & (0x1 << i) ) != 0 )
                bit = 1;
            else 
                bit = 0;
            if( bit ) /*i^th bit is 1*/
                send_bit_1(i, array);
            else
                send_bit_0(i, array);
            printf("%d\n", bit);
            sleep(1);
            sleep(1);
        }
    }
    printf("Finish sending\n");
    free(array_0);
}

