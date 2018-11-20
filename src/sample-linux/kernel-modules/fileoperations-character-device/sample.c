#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd;
    
    if ((fd = open("/dev/chrdev-fops", O_RDONLY, S_IRUSR)) != 0) {
        perror("open");
        exit(-1);
    } 
    
    close(fd);
}
