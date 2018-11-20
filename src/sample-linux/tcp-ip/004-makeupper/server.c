#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "socket.h"
#include "util.h"

#define PORTNO			5120
#define BUFSIZE			1024


void *client_thread_proc(void *param);
char *mystrupr(char *s);

int main(void)
{
    int result;
    int server_sock, client_sock;    
    pthread_t tid;
    struct sockaddr_in sin_client;
    socklen_t addr_len = sizeof(sin_client);
    
    if (init_tcp_server(PORTNO, 8, INADDR_ANY, &server_sock) == -1)
        exit_sys("init_tcp_server");

    printf("Waiting for connection...\n");

    for (;;) {
        if ((client_sock = accept(server_sock, (struct sockaddr *) &sin_client, &addr_len)) == -1) 
            exit_sys("accept");
        
        printf("Connected: %s:%d\n", inet_ntoa(sin_client.sin_addr), ntohs(sin_client.sin_port));        
        
        if ((result = pthread_create(&tid, NULL, client_thread_proc, &client_sock)) < -1)
            exit_sys_thread("pthread_create", result);

        pthread_detach(tid);
    }
    
    close(server_sock);    

    return 0;    
}

char *mystrupr(char *s)
{
    char *p = s;
    
    while (*p = toupper(*p))
        ++p;
    
    return s;
}

void *client_thread_proc(void *param)
{
    int sock = *(int *)param;
    uint32_t msg_len, host_len;
    int result;
    char buf[BUFSIZE];    
    
    for (;;) {        
        if ((result = read_socket(sock, &msg_len, sizeof(msg_len))) != sizeof(msg_len))
            break;        
        
        msg_len = ntohl(msg_len);    
        
        printf("msg_len:%u\n", msg_len);
        
        
        if (msg_len >= BUFSIZE)
            break;        
        
        if ((result = read_socket(sock, buf, msg_len)) != msg_len)
            break;        

        buf[result] = '\0';
        
        puts(buf);
        if (!strcmp(buf, "quit"))
            break;
        
        mystrupr(buf);
        
        msg_len = strlen(buf);        
        host_len = htonl(msg_len);        
        if (write_socket(sock, &host_len, sizeof(msg_len)) != sizeof(msg_len))
            break;
        
        if (write_socket(sock, buf, msg_len) != msg_len)
            break;
        
        puts(buf);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);    
}














