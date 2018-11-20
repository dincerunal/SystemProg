#include <sys/socket.h>
#include "socket.h"

int read_socket(int sock, void *buf, size_t count)
{
    int result;
    size_t left = count, index = 0;

    while (left > 0) {
        if ((result = recv(sock, (char *)buf + index, left, 0)) == -1)
            return -1;
        
        if (result == 0)
            break;
        
        index += result;
        left -= result;
    }

    return index;
}


int write_socket(int sock, const void *buf, size_t count)
{
    int result;
    size_t left = count, index = 0;

    while (left > 0) {
        if ((result = send(sock, (char *)buf + index, left, 0)) == -1)
            return -1;
        
        if (result == 0)
            break;
        
        index += result;
        left -= result;
    }

    return index;
}
