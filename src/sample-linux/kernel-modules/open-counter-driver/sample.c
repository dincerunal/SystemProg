/*---------------------------------------------------------------------------------------------------------------------
   
---------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

void exit_sys(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv)
{
    int fd;
    
    if ((fd = open("/dev/chrdev-opencounter", O_RDONLY)) <= 0)
        exit_sys("open");
    
    getchar();
    
   // close(fd);
    
    return 0;
}