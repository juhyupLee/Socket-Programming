#include "IOCP.h"
#include <stdio.h>
#include <stdlib.h>
#include <process.h> //? �Ƹ� �����带 ����°��̰���?
#include <WinSock2.h>
#include <Windows.h>
#include "interface_Func.h"

#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define BUF_SIZE 100
#define READ 3
#define WRITE 5

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA,*LPPER_HANDLE_DATA; // Handle Data

typedef struct
{
	OVERLAPPED overlapeed;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode; // Read or Write
}PER_IO_DATA, *LPPER_IO_DATA;

DWORD WINAPI EchoThreadMain(void* CompletionPortIO);
int IOCP(int argc, char* argv[])
{
	WSADATA wsaData;
	HANDLE hComPort; //CP ������Ʈ
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	//Socket Vs Handle?
	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	DWORD i;
	DWORD flags = 0;
	DWORD recvBytes = 0;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorMessage("WSAStartup Error");
	}

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	//�� INVALID_HANDLE_VALUE �� ��������??
	// NumberOfCurrentThreads �� 0���� �����ϸ�, ���� �ھ��� ����ŭ �����尡 CP ������Ʈ�� �Ҵ�� �� �ִ�.

	GetSystemInfo(&sysInfo);
	for (i = 0; i < sysInfo.dwNumberOfProcessors; ++i)
	{
		_beginthreadex(NULL, 0, (_beginthreadex_proc_type)EchoThreadMain, (LPVOID)&hComPort, 0, NULL);
	}

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (ULONG_PTR)handleInfo, 0);
		//Number of current Threads ��  0�����ϴ°� �����ǹ���?
		
		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapeed), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapeed), NULL);

	}

	return 0;
}

DWORD WINAPI EchoThreadMain(void* pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo=nullptr;
	LPPER_IO_DATA ioInfo=nullptr;
	DWORD flags = 0;

	while (1)
	{
		//������� �Ϸ�Ǹ� overlapped �� ���� �������޾ƿ��µ�,
		//�װ��� ����� overlapped�� ù������� IOINFO ����ü�� ����� io������ ������ �����´�.
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		sock = handleInfo->hClntSock;

		if (ioInfo->rwMode == READ)
		{
			puts("message received!");
			if (bytesTrans == 0) //EOF ���۽� 
			{
				closesocket(sock);
				free(handleInfo);
				free(ioInfo);
				continue;
			}

			memset(&(ioInfo->overlapeed), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans;
			ioInfo->rwMode = WRITE;
			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapeed), NULL);

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapeed), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapeed), NULL);
		}
		else
		{
			puts("message send!");
			free(ioInfo);
		}

	}
	

}
