#ifndef __LOCK_H__
#define __LOCK_H__

#define true 1
#define false 0

#ifdef DEBUG
#define dfprintf(fd, fmt, arg...)    fprintf(fd, fmt, ## arg)
#else
#define dfprintf(fd, fmt, arg...) 	
#endif

static inline int create_file(char* filename)
{
    FILE *file = fopen(filename, "a+"); 
    if( file == NULL )
    {
        fprintf(stderr, "Create file %s fails\n", filename);
        return false;
    }
    if( fclose(file) != 0 )
    {
        fprintf(stderr, "In create file, close the file %s fails\n", filename);
    } 
    return true;
}

static inline int remove_file(char* filename)
{
    int err = remove(filename);
    if( err == 0 )
        return true;
    if( err == -1 && errno == ENOENT)  //this file does NOT exist
    {
        dfprintf(stdout,"In remove file: this file %s does NOT exist; But remove also succeed\n", filename);
        return true;
    }
    //all others is removing fails
    return false;
}

static inline int is_file_exist(char* filename)
{
    int err = access(filename, F_OK);
    if( err == 0 )
    {
        return true;
    }else{
        return false;
    }
}

/*Lock the file and flag unconditionally unless fails in the fcntl()*/
static inline int lock_file(int fd, int *fd_lock_flag)
{
    int err = 0;
    struct flock fd_lock;
    int flag;
    flag = fcntl(fd, F_GETFL, 0);
    flag |= O_APPEND;   
    if( fcntl(fd, F_SETFL, flag) == -1)
    {
        fprintf(stderr, "Lock fd %d fails\n", fd);
        return false;
    }
    printf("lock %d\n", fd);
    return true;
/*    fd_lock.l_type = F_WRLCK;
    fd_lock.l_whence = SEEK_SET;
    fd_lock.l_start = 0;
    fd_lock.l_len = 0;
    err = fcntl(fd, F_SETLK, &fd_lock);
    if( err != 0)
    {   
        fprintf(stderr, "Try to lock file %d which was locked by another proc; err=%d, errno=%d\n", fd, err, errno); 
        return false;
    }else{
       *fd_lock_flag = 1;
        return true;
    }
*/   
}

/*Unlock fd and fd_lock_flag unconditionally unless fails*/
static inline int unlock_file(int fd, int *fd_lock_flag)
{
    int err = 0;
    struct flock fd_lock;
    int flag;
    
    flag = fcntl(fd, F_GETFL, 0);
    flag &= ~O_APPEND;
    if( fcntl(fd, F_SETFL, flag) == -1)
    {
        fprintf(stderr, "Unlock fd %d fails\n", fd);
        return false;
    }
    printf("unlock %d\n", fd);
    return true;
/*    
    fd_lock.l_type = F_UNLCK;
    fd_lock.l_whence = SEEK_SET;
    fd_lock.l_start = 0;
    fd_lock.l_len = 0;
    err = fcntl(fd, F_SETLK, &fd_lock);
    if( err != 0 ) 
    {
        fprintf(stderr, "Try to unlock file %d fails\n", fd);
        return false;
    }else{
        *fd_lock_flag = 0;
        return true;
    }
*/
}

/* If it's locked by itself and another proc, return true; otherwise;
 * Return false if fd or flag is unlocked
**/
static inline int is_locked(int fd, int *fd_lock_flag)
{
    int err = 0;
    struct flock fd_lock;
    int flag;
    flag = fcntl(fd, F_GETFL, 0);
    if(flag & O_APPEND)
        return true;
    else
        return false;
/*
    fd_lock.l_type = F_WRLCK;
    fd_lock.l_whence = SEEK_SET;
    fd_lock.l_start = 0;
    fd_lock.l_len = 0;
    err = fcntl(fd, F_GETLK, &fd_lock);
    //Could get the lock; Not locked by another proc
    if(fd_lock.l_type == F_UNLCK || *fd_lock_flag == 0) 
    {
        return false;
    }else{
        return true;
    }
*/
}

static inline int is_bit_1(int fd)
{
    int err = 0;
    struct flock fd_lock;
    fd_lock.l_type = F_WRLCK;
    fd_lock.l_whence = SEEK_SET;
    fd_lock.l_start = 0;
    fd_lock.l_len = 0;
    err = fcntl(fd, F_GETLK, &fd_lock);
    if(fd_lock.l_type == F_UNLCK) /*Could get the lock; Not locked by another proc*/
    {
        return false;
    }else{
        return true;
    }
   
}


static inline int set_bit(int fd)
{
    int err = 0;
    struct flock fd_lock;
    fd_lock.l_type = F_WRLCK;
    fd_lock.l_whence = SEEK_SET;
    fd_lock.l_start = 0;
    fd_lock.l_len = 0;
    err = fcntl(fd, F_SETLK, &fd_lock);
    if( err != 0)
    {
            fprintf(stderr, "Cannot set bit for fd_bit\n");
    }
   
}

static inline int clear_bit(int fd)
{
    int err = 0;
    struct flock fd_lock;
    fd_lock.l_type = F_UNLCK;
    fd_lock.l_whence = SEEK_SET;
    fd_lock.l_start = 0;
    fd_lock.l_len = 0;
    err = fcntl(fd, F_SETLK, &fd_lock);
    if( err != 0)
    {
            fprintf(stderr, "Cannot clear bit for fd_bit\n");
    }
}


#endif
