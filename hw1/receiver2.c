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

    FILE *fd_result = fopen("result.txt", "w");
    int err = 0;
    struct flock fd_fill_lock;
    int bit = 0;
    char buffer = 0;
    int i, j;
    /* Init files*/

/*    fd_empty = open("fd_empty.tmp", O_RDWR);
    fd_fill = open("fd_fill.tmp", O_RDWR);
    fd_bit = open("fd_bit.tmp", O_RDWR);
    fd_result = fopen("result.txt", "rw");
    
    if(fd_empty <= 0 || fd_fill <= 0 || fd_bit <= 0 || fd_result == NULL)
    {
        fprintf(stderr, "Open file fails, exit(1)\n");
        exit(1);
    }
*/
 //   lock_file(fd_fill, &fd_fill_lock_flag);
    create_file(fd_fill_name);
    
    while(1)
    {
//        while(is_locked(fd_fill, &fd_fill_lock_flag) == true);
        while(is_file_exist(fd_fill_name) == true);
//  {
//      printf("wait fd_fill release lock\n");
//  }
//        lock_file(fd_fill, &fd_fill_lock_flag);
        create_file(fd_fill_name);

/*        if(is_bit_1(fd_bit))
        {
            bit = 1;
            clear_bit(fd_bit);
        }else{
            bit = 0;
        }
*/
        if(is_file_exist(fd_bit_name))
        {
            bit = 1;
            remove_file(fd_bit_name);
        }else{
            bit = 0;
        }
        printf("%d", bit);
        if( fprintf(fd_result, "%c", buffer) <= 0)
        {
            fprintf(stderr, "Write to result.txt fails\n");
        }
        j = (j++) % 4;
        if(j == 0){
            printf(" ");
            fflush(stdout);
            if( fprintf(fd_result, " ") <= 0)
                fprintf(stderr, "Write to result.txt fails\n");
        }

//        if( fputc(bit, fd_result) == EOF)
        
//        unlock_file(fd_empty, &fd_empty_lock_flag);
         remove_file(fd_empty_name);
    }
/*    close(fd_empty);
    close(fd_fill);
    close(fd_bit);
*/
}
