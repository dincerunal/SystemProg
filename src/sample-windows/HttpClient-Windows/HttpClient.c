#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>

#define PORTNO		80				/* default well known http port */
#define HOSTNAME	"localhost"

void ExitSys(LPCSTR lpszMsg, int status, DWORD dwLastError);
int ReadLineUnbuffered(SOCKET sock, char *buf, size_t len);
int ReadSocket(SOCKET sock, void *buf, size_t len);

int main(void)
{
	WSADATA wsaData;
	struct sockaddr_in sinClient;
	SOCKET clientSock;
	struct hostent *host;
	char line[1024 + 1];
	char *buf;
	char *msgs[] = { "GET /Test.txtx HTTP/1.1\r\n", "Host: localhost\r\n", "\r\n", NULL };
	int result, i;
	long contentLength;

	if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
		ExitSys("WSAStartup", EXIT_FAILURE, result);

	if ((clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		ExitSys("socket", EXIT_FAILURE, WSAGetLastError());

	sinClient.sin_family = AF_INET;
	sinClient.sin_port = htons(PORTNO);
	sinClient.sin_addr.s_addr = inet_addr(HOSTNAME);
	if (sinClient.sin_addr.s_addr == INADDR_NONE) {
		if ((host = gethostbyname(HOSTNAME)) == NULL)
			ExitSys("gethostbyname", EXIT_FAILURE, WSAGetLastError());
		memcpy(&sinClient.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	}

	if (connect(clientSock, (struct sockaddr *) &sinClient, sizeof(sinClient)) == SOCKET_ERROR)
		ExitSys("connect", EXIT_FAILURE, WSAGetLastError());

	for (i = 0; msgs[i] != NULL; ++i) 
		if (send(clientSock, msgs[i], strlen(msgs[i]), 0) == SOCKET_ERROR) 	
			ExitSys("send", EXIT_FAILURE, WSAGetLastError());

	contentLength = -1;
	for (;;) {
		result = ReadLineUnbuffered(clientSock, line, 1024);
		if (result == SOCKET_ERROR)
			ExitSys("recv", EXIT_FAILURE, result);
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
		if (ReadSocket(clientSock, buf, contentLength) == SOCKET_ERROR)
			ExitSys("recv", EXIT_FAILURE, WSAGetLastError());

		for (i = 0; i < contentLength; ++i)
			putchar(buf[i]);
		putchar('\n');
		free(buf);
	}
	
	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);

	return 0;
}

int ReadLineUnbuffered(SOCKET sock, char *buf, size_t len)
{
	char ch1 = '\0', ch2;
	size_t i;
	int result;
	
	for (i = 0; i < len - 1; ++i) {
		if ((result = recv(sock, &ch2, 1, 0)) == SOCKET_ERROR)
			return SOCKET_ERROR;
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

int ReadSocket(SOCKET sock, void *buf, size_t len)
{
	int result;
	size_t index = 0;
	size_t left = len;
	char *cbuf = (char *)buf;
	
	while (left > 0) {
		if ((result = recv(sock, cbuf + index, left, 0)) == SOCKET_ERROR)
			return SOCKET_ERROR;
		if (result == 0)
			break;
		left -= result;
		index += result;
	}
	return index;
}

void ExitSys(LPCSTR lpszMsg, int status, DWORD dwLastError)
{
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL)) {
		fprintf(stderr, "%s: %s", lpszMsg, lpszErr);
		LocalFree(lpszErr);
	}

	exit(status);
}

