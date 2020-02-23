
#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <process.h> //? 아마 쓰레드를 만드는것이겟지?
#include <Windows.h>
#include "interface_Func.h"
#include "IOCP.h"

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
	HANDLE hComPort; //CP 오브젝트
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	//Socket Vs Handle?
	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int recvBytes, i,  flags = 0;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorMessage("WSAStartup Error");
	}

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);
	for (i = 0; i < sysInfo.dwNumberOfProcessors; ++i)
	{
		_beginthreadex(NULL, 0,(_beginthreadex_proc_type) EchoThreadMain, (LPVOID)&hComPort, 0, NULL);
		
	}

	return 0;
}

DWORD WINAPI EchoThreadMain(void* pComPort)
{

	DWORD temp;
	return temp;
}
