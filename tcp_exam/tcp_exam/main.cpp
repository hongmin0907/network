#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#ifndef __linux__
	#include <WinSock2.h>
	#include <Windows.h>
	#define perror(msg) winperror(msg)

	void winperror(const char* msg);
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#define PORT 1234

int main() {
	int sock, client_sock;
	struct sockaddr_in addr, client_addr;
	char buffer[1024];
	char csocket[] = "socket";
	int len, addr_len, recv_len;

#ifndef __linux__
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}

	memset(&addr, 0x00, sizeof(addr)); // 메모리 0x00으로 값으로 지정
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}

	if (listen(sock, 5) < 0) {
		perror("listen");
		return 1;
	}

	addr_len = sizeof(client_addr);

	printf("waiting for client..\n");

	while ((client_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len)) > 0) {
		printf("client ip : %s \n", inet_ntoa(client_addr.sin_addr));


		if ((recv_len = recv(client_sock, buffer, 1024, 0)) < 0) {
			perror("recv");
			break;
		}

		buffer[recv_len] = '\0';

		printf("received data: %s \n", buffer);

		send(client_sock, buffer, strlen(buffer), 0);

#ifndef __linux__
		closesocket(client_sock);
#else
		close(client_sock);
#endif
	}

#ifndef __linux__
	closesocket(sock);
	WSACleanup();
#else
	close(sock);
#endif

	return 0;
}

#ifndef __linux__
void winperror(const char* msg) {
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("%s : %s \n", msg, lpMsgBuf);

	LocalFree(lpMsgBuf);
}
#endif