#include <WinSock2.h>
#include <iostream>
#include "server_TCP_1.h"
#include "interface_Func.h"

using namespace std;
#define BUF_SIZE 1024
void Server_TCP_19(int argc, char* argv[])
{
	WSADATA wsaData;

	SOCKET hServSock, hCIntSock;

	PSOCKADDR_IN a;
	
	
	SOCKADDR_IN servAddr, cIntAddr;

	int cIntAdrSize;
	char message[BUF_SIZE];
	int strLen;

	if (argc != 2)
	{
		cout << "Usage:" << argv[0] << "<port>" << endl;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorMessage("WSAStart up() Error");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorMessage("socket() error");
	}
	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorMessage("bind() Error");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorMessage("lisetn() error");
	}

	cIntAdrSize = sizeof(cIntAddr);

	for (int i = 0; i < 5; ++i)
	{
		hCIntSock = accept(hServSock, (SOCKADDR*)& cIntAddr, &cIntAdrSize);
		cout << "----Accept Return Socket:----" << hCIntSock << endl;
		if (hCIntSock == -1)
		{
			ErrorMessage("accept() error");
		}
		else
		{
			cout << "Connected Client" << i + 1 << endl;
		}

		while ((strLen = recv(hCIntSock, message, BUF_SIZE, 0)) != 0)
		{
			send(hCIntSock, message, strLen, 0);
		}
		closesocket(hServSock);
		WSACleanup();
	

	}

}