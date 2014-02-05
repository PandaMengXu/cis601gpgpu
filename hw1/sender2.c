#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lock.h"

#define BUFFER_SIZE     8

int fd_empty_lock_flag = 0;
int fd_fill_lock_flag = 0;

static inline char get_rand_char()
{
    srand(time(NULL));
    return (char) rand() & 0x0FF;
}


void main()
{
    int fd_empty = 0, fd_fill = 0, fd_bit = 0;
    char fd_empty_name[] = "fd_empty.tmp";
    char fd_fill_name[] = "fd_fill.tmp";
    char fd_bit_name[] = "fd_bit.tmp";
    int err = 0, bit = 0, i = 0, j = 0;
    struct flock fd_fill_lock;
    char buffer = 0;

    while(1)
    {
        buffer = get_rand_char();
        for( i = 0; i < BUFFER_SIZE; i++)
        {
            if( (buffer & (1<<i)) == 0)
                bit = 0;
            else
                bit = 1;
            while(is_file_exist(fd_empty_name) == true);
            create_file(fd_empty_name);

            if( bit == 1)
            {
                create_file(fd_bit_name);
            }else{
                remove_file(fd_bit_name);
            }
            printf("%d", bit);
            j = (j++) % 4;
            if( j == 0 )
            {
                printf("\n");
                fflush(stdout);
            }
            remove_file(fd_fill_name);
        }       
    }
}
