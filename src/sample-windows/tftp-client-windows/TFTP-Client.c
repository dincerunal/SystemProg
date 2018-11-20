#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>

#define TFTP_PORTNO				69		/* Well known TFTP port */
#define HOSTNAME			"192.168.188.128" 

void SocketErrorExit(const char *msg);
int SendRRQ(SOCKET sock, const struct sockaddr_in *sinServer, const char *fileName, const char *type);
int SendAck(SOCKET sock, const struct sockaddr_in *sinServer, int blockNumber);
int ServerResponse(SOCKET sock, struct sockaddr_in *sinOther, char *buf, int size);
void DispPacket(int result, const char *buf, const struct sockaddr_in *sinOther, int size);

int main(void)
{
	WSADATA wsaData;
	int result;
	SOCKET clientSock;
	struct sockaddr_in sinServer, sinOther;
	int blockNumber;
	FILE *f;
	struct hostent *host;
	char buf[516], fileName[1024];
	DWORD dwTimeOut = 30000;
		
	if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		fprintf(stderr, "WSAStartup failed: %d\n", result);
		exit(EXIT_FAILURE);
	}

	if ((clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
		SocketErrorExit("socket");

	sinServer.sin_family = AF_INET;
	sinServer.sin_port = htons(TFTP_PORTNO);
	sinServer.sin_addr.s_addr = inet_addr(HOSTNAME);

	if (sinServer.sin_addr.s_addr == INADDR_NONE) {
		if ((host = gethostbyname(HOSTNAME)) == NULL) 
			SocketErrorExit("gethostbyname");
		memcpy(&sinServer.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	}

	printf("Enter file name to reeceive:");
	gets_s(fileName, 1024);

	if ((f = fopen(fileName, "wb")) == NULL) {
		fprintf(stderr, "cannot open file: %s\n", fileName);
		exit(EXIT_FAILURE);
	}
		
	if ((result = SendRRQ(clientSock, &sinServer, fileName, "octet")) == SOCKET_ERROR) 
		SocketErrorExit("SendRRQ");

	if (setsockopt(clientSock, SOL_SOCKET, SO_RCVTIMEO, &dwTimeOut, sizeof(dwTimeOut)) == SOCKET_ERROR)
		SocketErrorExit("setsockOpt");

	do {
		if ((result = ServerResponse(clientSock, &sinOther, buf, 1024)) == SOCKET_ERROR) {
			if (WSAGetLastError() == EWOULDBLOCK) {
				if (SendAck(clientSock, &sinServer, blockNumber + 1) == SOCKET_ERROR)
					SocketErrorExit("SendAck");
			}
			else
				SocketErrorExit("ServerResponse");
		}

		if (*(WORD *)buf == htons(3)) { /* DATA */
			blockNumber = ntohs(*(WORD *)(buf + 2));
		
			if (fwrite(buf + 4, 1, result - 4, f) != result - 4) {
				fprintf(stderr, "cannot write file!\n");
				exit(EXIT_FAILURE);
			}
			DispPacket(result, buf, &sinOther, 16);
			if (SendAck(clientSock, &sinOther, blockNumber) == SOCKET_ERROR) 
				SocketErrorExit("SendAck");
		}
		
	} while (result == 516);	/* 2 byte cmd + 2 byte block no + 512 byte data = 516 */
		
	closesocket(clientSock);
	WSACleanup();
	fclose(f);

	return 0;
}

void SocketErrorExit(const char *msg)
{
	DWORD dwLastError = WSAGetLastError();
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL)) {
		fprintf(stderr, "%s: %s", msg, lpszErr);
		LocalFree(lpszErr);
	}

	exit(EXIT_FAILURE);
}

int SendRRQ(SOCKET sock, const struct sockaddr_in *sinServer, const char *fileName, const char *type)
{
	int result;
	char buf[1024];

	*(WORD *)buf = htons(1);
	result = 2;
	result += sprintf(buf + result, "%s", fileName);
	buf[result++] = '\0';
	result += sprintf(buf + result, "%s", type);

	return sendto(sock, buf, result, 0, (const struct sockaddr *)sinServer, sizeof(*sinServer));
}

int SendAck(SOCKET sock, const struct sockaddr_in *sinServer, int blockNumber)
{
	char buf[4];

	*(WORD *)buf = htons(4);
	*(WORD *)(buf + 2) = htons(blockNumber);

	return sendto(sock, buf, 4, 0, (const struct sockaddr *)sinServer, sizeof(*sinServer));
}

int ServerResponse(SOCKET sock, struct sockaddr_in *sinOther, char *buf, int size)
{
	int addrLenClient;

	addrLenClient = sizeof(*sinOther);
	return recvfrom(sock, buf, size, 0, (struct sockaddr *) sinOther, &addrLenClient);
}

void DispPacket(int result, const char *buf, const struct sockaddr_in *sinOther, int size)
{
	int i;

	printf("%d byte received from %s:%d (%s)\n", result, inet_ntoa(sinOther->sin_addr), sinOther->sin_port, buf);

	for (i = 0; i < 16; ++i)
		printf("%02X ", buf[i]);
	printf("\n");
}


