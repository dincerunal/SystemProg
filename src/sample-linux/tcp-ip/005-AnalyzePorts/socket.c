#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
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

int is_port_available_v4(int port_no)
{
    int serverSock;
    struct sockaddr_in sinServer;
    int result;
    
    result = 1;
    
    if (port_no < 0 || port_no > 65535)
        return -1;

    if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
        return -1;

    sinServer.sin_family = AF_INET;
    sinServer.sin_port = htons(port_no);
    sinServer.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSock, (struct sockaddr *) &sinServer, sizeof(sinServer)) == -1)
        result = 0;
    
    close(serverSock);
    
    return result;
}














