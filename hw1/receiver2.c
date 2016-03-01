#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lock.h"

int fd_empty_lock_flag = 0;
int fd_fill_lock_flag = 0;


void main()
{
    int fd_empty = 0, fd_fill = 0, fd_bit = 0;
    char fd_empty_name[] = "fd_empty.tmp";
    char fd_fill_name[] = "fd_fill.tmp";
    char fd_bit_name[] = "fd_bit.tmp";

    int err = 0;
    struct flock fd_fill_lock;
    int bit = 0;
    char buffer = 0;
    int i, j;
    create_file(fd_fill_name);
    
    while(1)
    {
        while(is_file_exist(fd_fill_name) == true);
        create_file(fd_fill_name);

        if(is_file_exist(fd_bit_name))
        {
            bit = 1;
            remove_file(fd_bit_name);
        }else{
            bit = 0;
        }
        printf("%d", bit);
        j = (j++) % 4;
        if(j == 0){
            printf("\n");
            fflush(stdout);
        }

         remove_file(fd_empty_name);
    }
}
