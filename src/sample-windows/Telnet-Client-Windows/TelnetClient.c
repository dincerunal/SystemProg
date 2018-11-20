#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>
#include <wincon.h>

#define PORTNO		23				/* default well known telnet port */
#define HOSTNAME	 "192.168.188.128" 
#define BUFSIZE		1024

#define DO			0xFD
#define WONT		0xFC
#define WILL		0xFB
#define DONT		0xFE
#define IAC			0xFF

void ExitSys(LPCSTR lpszMsg, int status, DWORD dwLastError);
DWORD __stdcall ThreadProc(LPVOID param);
int ReadSocket(SOCKET sock, void *buf, size_t len);
int WriteSocket(SOCKET sock, const void *buf, size_t len);
void EnableVTMode(void);

int main(void)
{
	WSADATA wsaData;
	struct sockaddr_in sinClient;
	SOCKET clientSock;
	struct hostent *host;
	int result;
	DWORD threadId;
	HANDLE hThread;
	char ch;

	EnableVTMode();
		
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

	if ((hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)clientSock, 0, &threadId)) == NULL)
		ExitSys("CreateThread", EXIT_FAILURE, GetLastError());
	
	for (;;) {
		ch = getch();
		if (ch == '\r') {
			char buf[] = { '\r', '\n' };
			if (WriteSocket(clientSock, buf, 2) == 0)
				break;
			continue;
		}
		if (WriteSocket(clientSock, &ch, 1) == 0)
			break;
	}

	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);

	return 0;
}

int FillBuf(SOCKET sock, unsigned char *bufBeg, unsigned const char *bufCur, int copyLen)
{
	int result;
	
	memcpy(bufBeg, bufCur, copyLen);

	if ((result = recv(sock, bufBeg + copyLen, BUFSIZE, 0)) == SOCKET_ERROR)
		ExitSys("recv", EXIT_FAILURE, WSAGetLastError());

	return result;
}

DWORD __stdcall ThreadProc(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	unsigned char buf[BUFSIZE + 1];
	int n;
	int cur;
		
	for (;;) {
		if ((n = FillBuf(sock, buf, buf, 0)) == 0)
			break;
		cur = 0;
		while (cur < n) {
			if (buf[cur] == IAC) {
				if (n - cur < 3) {
					if ((n = FillBuf(sock, buf, &buf[cur], n - cur)) == 0)
						break;
					cur = 0;
				}
				switch (buf[cur + 1]) {
				case WILL:
					buf[cur + 1] = DONT; 
					break;
				case DO:
					buf[cur + 1] = WONT;
					break;
				case WONT:
					buf[cur + 1] = DONT;
					break;
				case DONT:
					buf[cur + 1] = WONT;
					break;
				}
				if (WriteSocket(sock, &buf[cur], 3) == 0)
					goto EXIT;
				cur += 3;
			}
			else {
				putchar(buf[cur]);
				++cur;
			}
		}
	EXIT:
		;
	}

	return 0;
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

void EnableVTMode(void)
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;

	if (hOut == INVALID_HANDLE_VALUE)
		ExitSys("GetStdHandle", EXIT_FAILURE, GetLastError());
	if (!GetConsoleMode(hOut, &dwMode))
		ExitSys("GetConsoleMode", EXIT_FAILURE, GetLastError());


	dwMode |= 4;
	if (!SetConsoleMode(hOut, dwMode))
		ExitSys("SetConsoleMode", EXIT_FAILURE, GetLastError());
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
			if (count == 0)
				return 0;
			bp = b;
		}
		ch = *bp++;
		*buf++ = ch;
		if (ch == '\n' && buf[-2] == '\r') {
			*buf = '\0';
			return buf - bufx;
		}
	}

	return SOCKET_ERROR;
}


