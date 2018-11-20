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
    char buf[1024];
    int n, i, count = 0;
    
    if ((fd = open("/dev/chrdev-random2", O_RDONLY)) <= 0)
        exit_sys("open");
    
    
    while ((n = read(fd, buf, 1024)) > 0) {
        buf[n] = '\0';
        puts(buf);
        ++count;
    }
    
    printf("count=%d\n", count);
    
    getchar();
    
    close(fd);
    
    return 0;
}