#include "udp_server_1.h"
#include <WinSock2.h>
#include "interface_Func.h"
enum
{
	eBUFSIZE =30
};
void Server_UDP_1(int argc, char* argv[])
{
	WSADATA wsaData;

	SOCKET servSock; //UDP�� ������ ������ �ƴϱ⶧���� �������ϸ� �ִ�.

	SOCKADDR_IN serverAddr, clntAddr;

	int clntAddrSize = 0;
	int strLen = 0;
	char message[eBUFSIZE];


	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		ErrorMessage("WStartup() Error");
		exit(1);

	}

	servSock = socket(PF_INET, SOCK_DGRAM, 0); // socket �Ű������� ���� ���ΰ� �ʿ� 
	if (servSock == INVALID_SOCKET)
	{
		ErrorMessage("socket() error)");
		exit(1);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET; // AF_INET�� ���� ���ΰ� �ʿ���
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //���� ȣ��Ʈ�� �ּҸ� ������
	serverAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)& serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		ErrorMessage("bind() Error");
		exit(1);
	}

	//TCP ip ���ٸ� listen ����� connect ��������ٵ� ���⼭�� �ƴ�)

	while (1)
	{
		clntAddrSize = sizeof(clntAddr);
		strLen = recvfrom(servSock, message, eBUFSIZE, 0, (SOCKADDR*)& clntAddr, &clntAddrSize);//recvfrom�Լ������� ������ �־��ٶ�, client �����̾ƴ϶�, server�Ǽ����� �־��ش�.
																							    //UDP  ������ TCP ���ϰ��޸� 1���� �ʿ��ѵ� �װ� ������������ �����̴�.
		sendto(servSock, message, strLen, 0, (SOCKADDR*)& clntAddr, sizeof(clntAddr));// ���������� snedto�Լ������� ������ �־��ٶ� client�����̾ƴ϶� server�� �������� �Ѵ�.
	}

	closesocket(servSock);
	WSACleanup();
	

}
