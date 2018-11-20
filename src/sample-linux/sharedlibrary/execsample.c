#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PATH    4096

void exit_sys(const char *msg);


int main(void)
{
    pid_t pid;
    char buf[MAX_PATH];
    
    if (getcwd(buf, MAX_PATH) < 0)
        exit_sys("getcwd");
    
    if (setenv("LD_LIBRARY_PATH", buf, 1) < 0)
        exit_sys("setenv");
    
    if ((pid = fork()) < 0)
        exit_sys("fork");
    
    if (pid == 0) {
        if (execl("./sample", "./sample", (char *)NULL) < 0)
            exit_sys("execl");
    }    
    if (wait(NULL) < 0)
        exit_sys("wait");   
    
    return 0;
}

void exit_sys(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}