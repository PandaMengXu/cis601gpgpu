#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define ARRAY_LENGTH    (4096+8)
#define ARRAY_EFFECTIVE_LENGTH  4096
 
#define SET_NUM         (64)
#define LINE_NUM        (8)
#define ELMT_PER_LINE   (8)


static inline float get_elapsed_usec(struct timeval before, struct timeval after)
{
    //dprintf("get_elapsed_usec: after.tv_sec = %ld, after.tv_usec = %ld; before.tv_sec=%ld, before.tv_usec=%ld\n", after.tv_sec, after.tv_usec, before.tv_sec, before.tv_usec);
    return ( ( (after.tv_sec - before.tv_sec) * 1000000.0 ) + 1.0 * ( after.tv_usec - before.tv_usec ) );
}



void main()
{
    unsigned long* array = NULL;
    int i = 0, try = 0, tmp;
    
    do{
        array = malloc(sizeof(unsigned long) * ARRAY_LENGTH);
    }while( array == NULL && try++ <= 10);
 
    while(1)
    {       
        for(i = 1 ; i< ARRAY_LENGTH - 1; i++)        
        {
            array[i] = i;
	    array[i + 1] = array[i] * 2;
            tmp = array[i - 1];
        }
         
    }
}

