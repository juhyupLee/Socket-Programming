#include "OverlappedIO.h"

#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <windows.h>

#define BUF_SIZE 1024
using namespace std;

int OverlappedIO(int argc, char* argv[])
{
	WSAData wsaData;
	SOCKADDR_IN servAddr;

	SOCKET hClntSock;
	int strLen = 0;
	int readLen = 0;


	char message[BUF_SIZE];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == -1)
	{
		cout << "WSAStart up Fail" << endl;
		return -1;
	}

	//생각해보니깐 Client는 WSASocket이 필요한가?
	hClntSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hClntSock == INVALID_SOCKET)
	{
		cout << "socket() Error" << endl;
		return -1;
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr.s_addr);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hClntSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		cout << "Connect() Error" << endl;
		return -1;
	}
	
	while (1)
	{
		cout << "Input Message(Q to Quit)" << endl;
		cin >> message;
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			break;
		}
		strLen = strlen(message);
		send(hClntSock, message, strLen, 0);


		readLen = 0;
		while (1)
		{
			readLen += recv(hClntSock, &message[readLen], BUF_SIZE - 1, 0);
			if (readLen >= strLen)
			{
				break;
			}
		}
		
		message[strLen] = 0;
		cout << "Message from server: " << message << endl;
	}
	closesocket(hClntSock);
	WSACleanup();
	return 0;

}
