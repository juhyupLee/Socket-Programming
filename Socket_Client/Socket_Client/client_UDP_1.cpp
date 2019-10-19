#include "client_UDP_1.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include "interface_Func.h"
using namespace std;

enum
{
	eBUF_SIZE=30
};
void Client_UDP_1(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serverAddr;// �� SOCKADDR�� �ٷ��ϸ���� SOCKADDR_IN���� ó�� ������?
	int strLen = 0;

	char message[eBUF_SIZE];
	if (argc != 3)
	{
		cout  << argv[0] << "   Port NO Error" << endl;
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		ErrorMessage("WSAStartup() Error");
		exit(1);
	}
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		ErrorMessage("socket() error");
	}
	
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(atoi(argv[2]));
	//���⼭ ���ε带 ���ϳ�??
	//�� ��°��?
	connect(sock, (SOCKADDR*)& serverAddr, sizeof(serverAddr));

	while (1)
	{
		fputs("Input Message: ", stdout);
		fgets(message, eBUF_SIZE, stdin);
		if (!strcmp("q\n", message) || !strcmp("Q\n", message))
		{
			break;
		}
		send(sock, message, strlen(message), 0);// sendto�� �Ⱦ��� ��°��?: ���� connect �� �Ἥ �׷���
		
		strLen = recv(sock, message, sizeof(message)-1, 0); // recvfrom�� �Ⱦ��� ��°��? : ���������� ���� connect���Ἥ �׷���.
		message[strLen] = 0;
		cout << "Message from server:" << message << endl;

	}
	closesocket(sock);
	WSACleanup();




}
