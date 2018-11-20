#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "socket.h"
#include "util.h"

#define PORTNO		5120
#define BUFSIZE		1024

int main(void)
{
    int server_sock;
    int result;
    char host_name[] = "127.0.0.1";
    struct hostent *host;
    uint32_t msg_len, host_len;

    if (connect_to_server(PORTNO,  host_name, &server_sock) == -1)
        exit_sys("connect_to_server");

    for (;;) {
        static char buf[100];

        printf("Text:");
        gets(buf);
        
        msg_len = strlen(buf);
        
        host_len = htonl(msg_len);
        
        if (write_socket(server_sock, &host_len, sizeof(host_len)) != sizeof(host_len))
            exit_sys("write_socket");        
        
        
        if (write_socket(server_sock, buf, msg_len) != msg_len)
            exit_sys("write_socket");                
        
        if (!strcmp(buf, "quit"))
            break;
        
        if ((result = read_socket(server_sock, &msg_len, sizeof(msg_len))) != sizeof(msg_len))
            exit_sys("read_socket");
        
        msg_len = ntohl(msg_len);
        
        if ((result = read_socket(server_sock, buf, msg_len)) != msg_len)
            exit_sys("read_socket");        
        
        
        buf[result] = '\0';
        
        puts(buf);
    }

    shutdown(server_sock, SHUT_RDWR);
    close(server_sock);

    return 0;
}

