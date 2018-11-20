#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void exit_sys(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void exit_sys_thread(const char *msg, int errnum)
{
    fprintf(stderr, "%s:%s\n", msg, strerror(errnum));
    exit(EXIT_FAILURE);
}
