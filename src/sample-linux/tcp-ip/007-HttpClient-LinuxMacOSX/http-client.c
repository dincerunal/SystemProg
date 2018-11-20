#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORTNO			80             /* default well known http port */
#define HOSTNAME		"www.csystem.org"

void err_exit(const char *msg);
int read_line_unbuffered(int sock, char *buf, size_t len);
int read_socket(int sock, void *buf, size_t len);

int main(void)
{
	int result;
	struct sockaddr_in sinClient;
	int clientSock;
	struct hostent *host;
	char line[1024];
	char *buf;
	char *msgs[] = { "GET /Test.txt HTTP/1.1\r\n", "Host: localhost\r\n", "\r\n", NULL };
	int i;
	long contentLength;

	if ((clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		err_exit("socket");

	sinClient.sin_family = AF_INET;
	sinClient.sin_port = htons(PORTNO);
	sinClient.sin_addr.s_addr = inet_addr(HOSTNAME);
	if (sinClient.sin_addr.s_addr == INADDR_NONE) {
		if ((host = gethostbyname(HOSTNAME)) == NULL)
			err_exit("gethostbyname");
		memcpy(&sinClient.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	}
	if (connect(clientSock, (struct sockaddr *) &sinClient, sizeof(sinClient)) == -1)
		err_exit("connect");

	for (i = 0; msgs[i] != NULL; ++i)
		if (send(clientSock, msgs[i], strlen(msgs[i]), 0) == -1)
			err_exit("send");

	contentLength = -1;
	for (;;) {
		result = read_line_unbuffered(clientSock, line, 1024);
		if (result == -1)
			err_exit("read_line_unbuffered");
		if (result == 0)		/* socket is closed by server */
			break;
		if (result == 2)		/* header lines ended */
			break;

		if (strstr(line, "Content-Length: "))
			contentLength = strtol(line + 15, NULL, 10);
		printf(line);
	}

	if (contentLength != -1) {
		if ((buf = (char *)malloc(contentLength)) == NULL) {
			fprintf(stderr, "cannot allocate memory!\n");
			exit(EXIT_FAILURE);
		}
		if (read_socket(clientSock, buf, contentLength) == -1)
			err_exit("read_socket");

		for (i = 0; i < contentLength; ++i)
			putchar(buf[i]);
		putchar('\n');
		free(buf);
	}
	
	shutdown(clientSock, SHUT_RDWR);
	close(clientSock);

	return 0;
}

int read_line_unbuffered(int sock, char *buf, size_t len)
{
	char ch1 = '\0', ch2;
	size_t i;
	int result;

	for (i = 0; i < len - 1; ++i) {
		if ((result = recv(sock, &ch2, 1, 0)) == -1)
			return -1;
		if (result == 0)
			return 0;
		buf[i] = ch2;
		if (ch2 == '\n' && ch1 == '\r') {
			buf[i + 1] = '\0';
			return i + 1;
		}
		else
			ch1 = ch2;
	}
	return i;
}

int read_socket(int sock, void *buf, size_t len)
{
	int result;
	size_t index = 0;
	size_t left = len;
	char *cbuf = (char *)buf;

	while (left > 0) {
		if ((result = recv(sock, cbuf + index, left, 0)) == -1)
			return -1;
		if (result == 0)
			break;
		left -= result;
		index += result;
	}
	return index;
}

void err_exit(const char *msg)
{
	perror(msg);

	exit(EXIT_FAILURE);
}
