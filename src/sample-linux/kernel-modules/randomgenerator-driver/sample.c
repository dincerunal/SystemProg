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
    int n, i;
    
    if ((fd = open("/dev/chrdev-random", O_RDONLY)) <= 0)
        exit_sys("open");
    
    
    for (i = 0; i < 10; ++i) {
        n = read(fd, buf, 1024);    
        buf[n] = '\0';    
        puts(buf);
    }
    
    getchar();
    
    close(fd);
    
    return 0;
}