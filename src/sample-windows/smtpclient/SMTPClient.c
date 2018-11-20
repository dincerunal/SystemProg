#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>
#include <wincon.h>

#define SMTP_PORT			587						/* default well known port for SMTP Server */
#define HOSTNAME			"mail.csystem.org"
#define BUFSIZE				1024

void ExitSys(LPCSTR lpszMsg, int status, DWORD dwLastError);
DWORD __stdcall ThreadProc(LPVOID param);
int ReadSocket(SOCKET sock, void *buf, size_t len);
int WriteSocket(SOCKET sock, const void *buf, size_t len);
int ReadLineSocket(SOCKET sock, char *buf, size_t len);

char *g_messages[] = {
	"HELO csystem.org\r\n",
	"MAIL FROM: dropbox@csystem.org\r\n",
	"AUTH LOGIN\r\n",
	"ZHJvcGJveEBjc3lzdGVtLm9yZw==\r\n",
	"Y3N5c3RlbS0xOTkz\r\n",
	"RCPT TO: info@kanattim.com\r\n",
	"DATA\r\n",
	"From: Kaan Aslan\r\nTo: Ali Serçe\r\nSubject: Deneme-3\r\nBu bir denemedir.\r\n.\r\n",
	"QUIT\r\n", 
	NULL
};

int main(void)
{
	SOCKET clientSock;
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

	if ((clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		ExitSys("socket", EXIT_FAILURE, WSAGetLastError());

	sinClient.sin_family = AF_INET;
	sinClient.sin_port = htons(SMTP_PORT);
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
	
	WaitForSingleObject(hThread, INFINITE);

	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);

	return 0;
}

DWORD __stdcall ThreadProc(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	unsigned char buf[BUFSIZE + 1];
	int n, code;
	int i;

	i = 0;
	for (;;) {
		if ((n = ReadLineSocket(sock, buf, BUFSIZE)) == SOCKET_ERROR)
			break;
		printf(buf);
		if (g_messages[i] == NULL)
			break;
		if (n == 0)
			break;
		WriteSocket(sock, g_messages[i], strlen(g_messages[i]));
		++i;
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
		if (ch == '\n') {
			*buf = '\0';
			return buf - bufx;
		}
	}

	return SOCKET_ERROR;
}