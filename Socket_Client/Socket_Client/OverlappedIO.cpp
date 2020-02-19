#include "OverlappedIO.h"

#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <ws2tcpip.h>

#include <windows.h>


using namespace std;

int OverlappedIO(int argc, char* argv[])
{
	WSAData wsaData;
	SOCKADDR_IN servAddr;

	SOCKET hClntSock;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == -1)
	{
		cout << "WSAStart up Fail" << endl;
		return -1;
	}

	hClntSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (hClntSock == INVALID_SOCKET)
	{
		cout << "Socket Fail" << endl;
	}

	//inet_pton ������� ����? �� �ȵ���?
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr.s_addr);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = atoi(argv[2]);

	//Connect�� ������ Ŭ���̾�Ʈ? ����?

	if (-1 == connect(hClntSock, (SOCKADDR*)&servAddr, sizeof(servAddr)))
	{
		cout << "Connect Fail" << endl;
		return -1;
	}


	//connect�� �� ��ȯ����? ������ ������ ��ȯ�߳�?

	// Ŭ���̾�Ʈ�� ���ϻ����ҋ�, �ʿ䰡������?





	// Ŭ���̾�Ʈ���� �ּ� ���ε��� �ߴ���? -> �׷��Ű��� ������ �ּҸ� �˾ƾ� ����̵Ǵϱ�
	//--> �ƴϴ� Ŭ���̾�Ʈ������ �ּҿ� ���ε����� �ʴ´�. ������ Ŭ���̾�Ʈ�� �ּҷ� ������ �����°��̾ƴϱ� �����̴�
	// Ŭ���̾�Ʈ�� ������ ��û���ϰ�, ������ �� ��û�� Ŭ���̾�Ʈ�� ���ϸ������� �󸶵��� ����� �� �� �ֱ⶧���� 
	// ��ġ�� ó�� connect�� �Ƿ��� ������ �ּҸ� �˾ƾ��Ѵ�.


	// �ߴٸ� �� ����?
	// ������ �ּҸ� �˾ƾ���
	// INADDR ANY?
	// Ŭ���̾�Ʈ ������ ��������?
	// ������ ������ �������� �����ϰ�, Ŭ���̾�Ʈ�� ������ Ŭ���̾�Ʈ�� ����


	return 0;
}
