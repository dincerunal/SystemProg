#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "util.h"

#define PORTNO		5120
#define BUFSIZE		1024

int main(void)
{
	int serverSock;
	int result;
	struct sockaddr_in sinServer;
	char hostName[] = "127.0.0.1";
	struct hostent *host;

	if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
            exit_sys("socket");

	sinServer.sin_family = AF_INET;
	sinServer.sin_port = htons(PORTNO);

	if ((sinServer.sin_addr.s_addr = inet_addr(hostName)) == INADDR_NONE) {
            if ((host = gethostbyname(hostName)) == NULL) 
                exit_sys("gethostbyname");
            
            memcpy(&sinServer.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	}

	if (connect(serverSock, (struct sockaddr *)&sinServer, sizeof(sinServer)) == -1) 
            exit_sys("connect");        

	for (;;) {
		static char buf[100];

		printf("Text:");
		gets(buf);
		if (send(serverSock, buf, strlen(buf), 0) == -1)
                    exit_sys("connect");  
                
		if (!strcmp(buf, "quit"))
                    break;
	}

	shutdown(serverSock, SHUT_RDWR);
	close(serverSock);

	return 0;
}

