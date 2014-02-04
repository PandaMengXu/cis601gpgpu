#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define ARRAY_LENGTH    (4096+8)
#define ARRAY_EFFECTIVE_LENGTH  4096
 
#define SET_NUM         (64)
#define LINE_NUM        (8)
#define ELMT_PER_LINE   (8)

void main()
{
    unsigned long* array = NULL;
    int i = 0, try = 0;
    
    do{
        array = malloc(sizeof(unsigned long) * ARRAY_LENGTH);
    }while( array == NULL && try++ <= 10);
        
    for(i = 0 ; i< ARRAY_LENGTH; i++)        
        array[i] = i;
}

