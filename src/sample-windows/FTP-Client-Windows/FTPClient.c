#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>
#include <wincon.h>

#define CONTROL_PORTNO		21				/* default well known port for FTP command connection */
#define HOSTNAME			"192.168.188.128" 
#define BUFSIZE				1024

void ExitSys(LPCSTR lpszMsg, int status, DWORD dwLastError);
DWORD __stdcall ThreadProc(LPVOID param);
int ReadSocket(SOCKET sock, void *buf, size_t len);
int WriteSocket(SOCKET sock, const void *buf, size_t len);
int ReadLineSocket(SOCKET sock, char *buf, size_t len);

void ParseCmd(const char *cmdLine);
void EnteringPassiveMode(char *str);
void GetData(void);
void DoListDir(void);
void RetrieveFile(void);
void CloseService(void);

SOCKET g_clientSock;
SOCKET g_dataSock;
char g_cmdLine[BUFSIZE];
char *g_cmdParams[10];
int g_cmdParamCount;

int main(void)
{
	WSADATA wsaData;
	struct sockaddr_in sinClient;
	struct hostent *host;
	int result;
	DWORD threadId;
	HANDLE hThread;
	char cmdLine[BUFSIZE];
	size_t len;
		
	if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
		ExitSys("WSAStartup", EXIT_FAILURE, result);

	if ((g_clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		ExitSys("socket", EXIT_FAILURE, WSAGetLastError());

	sinClient.sin_family = AF_INET;
	sinClient.sin_port = htons(CONTROL_PORTNO);
	sinClient.sin_addr.s_addr = inet_addr(HOSTNAME);
	if (sinClient.sin_addr.s_addr == INADDR_NONE) {
		if ((host = gethostbyname(HOSTNAME)) == NULL)
			ExitSys("gethostbyname", EXIT_FAILURE, WSAGetLastError());
		memcpy(&sinClient.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	}

	if (connect(g_clientSock, (struct sockaddr *) &sinClient, sizeof(sinClient)) == SOCKET_ERROR)
		ExitSys("connect", EXIT_FAILURE, WSAGetLastError());

	if ((hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)g_clientSock, 0, &threadId)) == NULL)
		ExitSys("CreateThread", EXIT_FAILURE, GetLastError());
	
	for (;;) {
		gets_s(cmdLine, BUFSIZE);
		ParseCmd(cmdLine);
		strcat(cmdLine, "\r\n");
		len = strlen(cmdLine);
		if (WriteSocket(g_clientSock, cmdLine, len) != len)
			break;
	}

	shutdown(g_clientSock, SD_BOTH);
	closesocket(g_clientSock);

	return 0;
}

DWORD __stdcall ThreadProc(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	unsigned char buf[BUFSIZE + 1];
	int n, code;
	int i;
				
	for (;;) {
		if ((n = ReadLineSocket(sock, buf, BUFSIZE)) == SOCKET_ERROR)
			break;
		if (n == 0)
			break;
		buf[n] = '\0';
		printf("%s", buf);
		code = strtol(buf, NULL, 10);
		switch (code) {
			case 227:		/* Entering Passive Mode */
				EnteringPassiveMode(buf);
				break;
			case 150:		/* Ok */
				GetData();
				break;
			case 221:		/* Service closing */
				CloseService();
				break;
			}
	}

	return 0;
}

void ParseCmd(const char *cmdLine)
{
	char *str;

	strcpy(g_cmdLine, cmdLine);

	g_cmdParamCount = 0;
	for (str = strtok(g_cmdLine, " "); str != NULL; str = strtok(NULL, " "))
		g_cmdParams[g_cmdParamCount++] = str;

	g_cmdParams[g_cmdParamCount] = NULL;
}

void EnteringPassiveMode(char *str)
{
	char *strBeg, *strEnd, *tok;
	int count, portNo;
	struct sockaddr_in sinClient;
	struct hostent *host;

	if ((strBeg = strchr(str, '(')) == NULL) {
		fprintf(stderr, "server error!\n");
		exit(EXIT_FAILURE);
	}
	if ((strEnd = strchr(str, ')')) == NULL) {
		fprintf(stderr, "server error!\n");
		exit(EXIT_FAILURE);
	}
	*strEnd = '\0';

	for (tok = strtok(strBeg, ","), count = 0; tok != NULL; tok = strtok(NULL, ","), ++count) {
		if (count == 4)
			portNo = atoi(tok) * 256;
		else if (count == 5)
			portNo += atoi(tok);
	}
	if ((g_dataSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		ExitSys("socket", EXIT_FAILURE, WSAGetLastError());
	sinClient.sin_family = AF_INET;
	sinClient.sin_port = htons(portNo);
	sinClient.sin_addr.s_addr = inet_addr(HOSTNAME);
	if (sinClient.sin_addr.s_addr == INADDR_NONE) {
		if ((host = gethostbyname(HOSTNAME)) == NULL)
			ExitSys("gethostbyname", EXIT_FAILURE, WSAGetLastError());
		memcpy(&sinClient.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	}
	if (connect(g_dataSock, (struct sockaddr *) &sinClient, sizeof(sinClient)) == SOCKET_ERROR)
		ExitSys("connect", EXIT_FAILURE, WSAGetLastError());
}

void GetData(void)
{
	if (!strcmp(g_cmdParams[0], "LIST"))
		DoListDir();
	else if (!strcmp(g_cmdParams[0], "RETR"))
		RetrieveFile();

	shutdown(g_dataSock, SD_BOTH);
	closesocket(g_dataSock);
}

void DoListDir(void)
{
	unsigned char buf[BUFSIZE + 1];
	int n;

	for (;;) {
		if ((n = ReadLineSocket(g_dataSock, buf, BUFSIZE)) == SOCKET_ERROR)
			break;
		if (n == 0)
			break;
		printf("%s", buf);
	}
}

void RetrieveFile(void)
{
	FILE *f;
	char buf[4096];
	int result;

	if ((f = fopen(g_cmdParams[1], "w")) == NULL) {
		fprintf(stderr, "cannot open file: %s\n", g_cmdParams[1]);
		return;
	}

	while ((result = recv(g_dataSock, buf, 4096, 0)) != 0 && result != SOCKET_ERROR)
		if (fwrite(buf, 1, result, f) != result) {
			fprintf(stderr, "cannot write file: %s\n", g_cmdParams[1]);
			break;
		}
	fclose(f);

	printf("File saved: %s\n", g_cmdParams[1]);
}

void CloseService(void)
{
	shutdown(g_clientSock, SD_BOTH);
	closesocket(g_clientSock);

	shutdown(g_dataSock, SD_BOTH);
	closesocket(g_dataSock);

	exit(EXIT_SUCCESS);
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

int WriteSocket(SOCKET sock, const void *buf, size_t len)
{
	int result;
	size_t index = 0;
	size_t left = len;
	char *cbuf = (char *)buf;

	while (left > 0) {
		if ((result = send(sock, cbuf + index, left, 0)) == SOCKET_ERROR)
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

int ReadLineSocket(SOCKET sock, char *buf, size_t len)
{
	char *bufx = buf;
	static char *bp;
	static int count = 0;
	static char b[2048];
	char ch;
	
	while (--len > 0) {
		if (--count <= 0) {
			count = recv(sock, b, sizeof(b), 0);
			if (count == SOCKET_ERROR) 
				return -1;
			if (count == 0 )
				return 0;
			bp = b;
		}
		ch = *bp++;
		*buf++ = ch;
		if (ch == '\n') {
			*buf = '\0';
			return buf - bufx;
		}
	}
	
	return SOCKET_ERROR;
}