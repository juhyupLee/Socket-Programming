#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include "interface_Func.h"

using namespace std;
enum
{
	eBUF_SIZE =30
};

void Client_TCP_1(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[eBUF_SIZE];
	int strLen;
	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		cout << "Usage:" << argv[0] << "<port>" << endl;
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorMessage("WSAStartup() Error");
	}
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorMessage("socket Error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorMessage("connect() Error");
	}
	else
	{
		cout << "Connected...." << endl;
	}

	while (1)
	{

		fputs("Input message(Q to quit): ", stdout);
		fgets(message, eBUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			break;
		}
		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, eBUF_SIZE - 1, 0);
		message[strLen] = 0;
		cout << "Message from server :" << message << endl;
	}
	closesocket(hSocket);
	WSACleanup();

}