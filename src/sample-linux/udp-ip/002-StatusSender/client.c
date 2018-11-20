#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "util.h"

#define PORTNO				5050
#define HOSTNAME		        "192.168.0.124"


int main(void)
{
    int result;
    struct sockaddr_in sinClient;
    int addrLenClient;
    int clientSock;
    struct hostent *host;
    int status = 1;

    if ((clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
            exit_sys("socket");

    sinClient.sin_family = AF_INET;
    sinClient.sin_port = htons(PORTNO);
    sinClient.sin_addr.s_addr = inet_addr(HOSTNAME);
    if (sinClient.sin_addr.s_addr == INADDR_NONE) {
        if ((host = gethostbyname(HOSTNAME)) == NULL) 
            exit_sys("gethostbyname");

        memcpy(&sinClient.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
    }

    for (;;) {        
        if (sendto(clientSock, &status, sizeof(int),  0, (struct sockaddr *)&sinClient, sizeof(sinClient))== -1) {
            fprintf(stderr, "send failed!\n");
            exit(EXIT_FAILURE);
        }        
        
        sleep(1);
    }

    close(clientSock);

    return 0;
}