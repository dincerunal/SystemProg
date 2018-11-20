#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "util.h"

#define PORTNO	  5050
#define BUF_LEN   8192


int main(void)
{	
    int result;
    int serverSock;
    struct sockaddr_in sinServer;
    struct sockaddr_in sinClient;
    int addrLenClient;
    int status = 1;

    if ((serverSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) 
        exit_sys("socket");

    sinServer.sin_family = AF_INET;
    sinServer.sin_port = htons(PORTNO);
    sinServer.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSock, (struct sockaddr *) &sinServer, sizeof(sinServer)) == -1) 
        exit_sys("socket");

    for (;;) {
        addrLenClient = sizeof(sinClient);
        result = recvfrom(serverSock, &status, sizeof(int), 0, (struct sockaddr *) &sinClient, &addrLenClient);
        printf("%d byte received from %s:%d (%d)\n", result, inet_ntoa(sinClient.sin_addr), ntohs(sinClient.sin_port), status);
    }
    
    close(serverSock);

	return 0;
}