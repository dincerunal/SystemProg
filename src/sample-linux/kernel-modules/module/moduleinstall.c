#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char insbuf[1024] = "insmod ";
    char rmbuf[1024] = "rmmod ";
    if (argc != 2) {
        fprintf(stderr, "wrong usage\n");
        exit(-1);
    }
    
    strcat(insbuf, argv[1]);
    strcat(rmbuf, argv[1]);
    system(insbuf);
    system(rmbuf);
    
    return 0;
}