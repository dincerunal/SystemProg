#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h> 

#include "socket.h"
#include "util.h"

#define PORTNO			5120
#define BUFSIZE			1024

void *client_thread_proc(void *param);

int main(void)
{
    int result;
    int serverSock, clientSock;
    
    pthread_t tid;

    struct sockaddr_in sinServer, sinClient;
    socklen_t addrLen = sizeof(sinClient);    

    if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
        exit_sys("socket");

    sinServer.sin_family = AF_INET;
    sinServer.sin_port = htons(PORTNO);
    sinServer.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSock, (struct sockaddr *) &sinServer, sizeof(sinServer)) == -1)
        exit_sys("bind");

    if (listen(serverSock, 8) == -1)
        exit_sys("listen");

    printf("Waiting for connection...\n");

    for (;;) {
        if ((clientSock = accept(serverSock, (struct sockaddr *) &sinClient, &addrLen)) == -1) 
            exit_sys("accept");

        printf("Connected: %s:%d\n", inet_ntoa(sinClient.sin_addr), ntohs(sinClient.sin_port));
        
        if ((result = pthread_create(&tid, NULL, client_thread_proc, &clientSock)) < -1)
            exit_sys_thread("pthread_create", result);

        pthread_detach(tid); /* zombie thread olmasın diye */
    }
    
    close(serverSock);
    

    return 0;
    
    
}

void *client_thread_proc(void *param)
{
    int sock = *(int *)param;
    size_t msg_len;
    int result;
    char buf[BUFSIZE];
    
    
    for (;;) {                
        if ((result = read_socket(sock, &msg_len, sizeof(msg_len))) == -1)
            break;
        
        if (msg_len > BUFSIZE)
            break;
        
        if ((result = read_socket(sock, buf, msg_len)) == -1)
            break;                
            
        if (result == 0)
            break;

        buf[result] = '\0';
        if (!strcmp(buf, "quit"))
            break;
        
        puts(buf);
    }


    shutdown(sock, SHUT_RDWR);
    close(sock);
    
    
}














