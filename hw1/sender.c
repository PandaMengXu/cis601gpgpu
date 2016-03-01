#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <inttypes.h>

#define ARRAY_LENGTH            (4096 + 8)
#define ARRAY_EFFECTIVE_LENGTH  4096

#define SET_NUM     (64)
#define LINE_NUM    (8)
#define ELMT_PER_LINE   (8)

#define BITS_WIDTH       32
#define BUFFER_LENGTH   (32/8)

static inline int get_array_index(int set, int line)
{
    return set * ELMT_PER_LINE + line * SET_NUM * ELMT_PER_LINE;
}

static inline void send_bit_1(int bit_index, unsigned long* array)
{
    int line = 0, k = 0, tmp;
    int set = bit_index * 2;
    for( line = 0; line < LINE_NUM; line++)
    {
        for( k = get_array_index(set, line); 
                k < get_array_index(set, line) + ELMT_PER_LINE; k++)
        {
            array[k] = k*2; /*evict out cache*/
            tmp = array[k] + k - 3;
        }
    }
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
    unsigned long* array = NULL;
    int set = 0, line = 0, k = 0;
    int start = 0, end = 0;
    int try = 0, i = 0, j = 0;
    int bit = 0;
    uint32_t buffer = 0;
    int send_times = 3;
    
    
    /* Test: unsigned long must be 64bit*/
    if( sizeof(unsigned long) != 8 || sizeof(unsigned int) != 4)
    {
        fprintf(stderr, "Error: unsigned long must be 8 Byte and unsigned int must be 4 Byte for the program to work\n");
        exit(1);
    }

    /* allocate array and load L1 data cache*/
    do
    {
        array = malloc(sizeof(unsigned long) * ARRAY_LENGTH);
    }while( array == NULL && try++ <= 10);
    if( array == NULL)
    {
        fprintf(stderr, "Error: cannot allocate array size:%d\n", ARRAY_LENGTH);
        exit(2);
    }

    /* Allign the start of the array to be the start of a cache line*/
    if( ( (unsigned long)array & 0x038UL ) != 0) /*Start of array is not start of a cache line*/
    {
        start = (0x08 - (( (unsigned long)array & 0x038UL ) >> 0x3));
    }else{
        start = 0;
    }
    printf("Allign the array with the cache line: start element's index is:%d\n", start);

    /* Generate the bits to send*/
    for( i = 0; i < BUFFER_LENGTH; i++ )
    {
        buffer |= ( get_rand_char() << (i*8) );
    }
    printf("Sender send %d bits data:%#010x\n", BITS_WIDTH, buffer);
    
    sleep(1);
    for( j = 0; j < send_times; j++)
	{
		for( i = 0; i < BITS_WIDTH; i++ )
		{
			if( buffer & ( 0x1 << i )) /*i^th bit is 1*/
				send_bit_1(i, array);
			else
				send_bit_0(i, array);
		}
	}
    printf("Finish sending\n");
    free(array);
}

