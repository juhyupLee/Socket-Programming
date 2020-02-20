#include "OverlappedIO.h"
#include <WinSock2.h>
#include <iostream>
#include <cstdlib>
#include "interface_Func.h"
using namespace std;

#define BUF_SIZE 1024

typedef struct 
{
	SOCKET recvSock;
	WSABUF wsaBuf;
	char buf[BUF_SIZE];

}PER_IO_DATA,*LP_PER_IO_DATA;
void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags);
void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags);

int OverlappedIO(int argc, char* argv[])
{

	WSAData wsaData;
	SOCKADDR_IN sockAddr,recvAddr;

	SOCKET hLisnSock,hRecvSock;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == -1)
	{
		cout << "WSAStart Up Fail" << endl;
		return -1;
	}

	hLisnSock =WSASocket(PF_INET, SOCK_STREAM, 0 , NULL , 0 ,WSA_FLAG_OVERLAPPED);
	u_long nonBlockingMode = 1;
	ioctlsocket(hLisnSock, FIONBIO, &nonBlockingMode); // non blocking mode accept

	if (hLisnSock == INVALID_SOCKET)
	{
		cout << "Socket Fail" << endl;
		return -1;
	}

	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(atoi(argv[1]));
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(hLisnSock, (sockaddr*)& sockAddr, sizeof(sockAddr)))
	{
		cout << "Bind Error" << endl;
		return -1;
	}

	if (listen(hLisnSock, 5) == SOCKET_ERROR)
	{
		cout << "Listen Error" << endl;
		return -1;
	}
	int szClntAddr = sizeof(recvAddr);


	while (1)
	{
		SleepEx(100, 1);
		hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAddr, &szClntAddr);
		if (hRecvSock == INVALID_SOCKET)
		{
			if (WSAEWOULDBLOCK == WSAGetLastError())
			{
				//cout << "WSAWOULDBLOCK" << endl;
				continue;
			}
			else
			{
				ErrorMessage("Accept Error");
			}
		}
		cout << "Connected Client...." << endl;
		LP_PER_IO_DATA pioData = (LP_PER_IO_DATA)malloc(sizeof(PER_IO_DATA));

		pioData->recvSock = hRecvSock;
		pioData->wsaBuf.buf = pioData->buf;
		pioData->wsaBuf.len = BUF_SIZE;

		WSAOVERLAPPED* wsaOverlapped = (WSAOVERLAPPED*)malloc(sizeof(WSAOVERLAPPED));

		wsaOverlapped->hEvent = (HANDLE)pioData;

		DWORD recvByte = 0;
		DWORD dwFlag = 0;

		WSARecv(hRecvSock, &(pioData->wsaBuf), 1, &recvByte, &dwFlag, wsaOverlapped, ReadCompRoutine);
	}


	closesocket(hRecvSock);
	closesocket(hLisnSock);

	WSACleanup();


	return 0;
}

void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	
	LP_PER_IO_DATA temp = (LP_PER_IO_DATA)lpOverlapped->hEvent;
	SOCKET hRecvSocket = temp->recvSock;
	DWORD sentByte = 0;
	DWORD dwFlag = 0;
	
	//Client -> EOF
	if (szRecvBytes == 0)
	{
		free(temp);
		free(lpOverlapped);
		closesocket(hRecvSocket);
	}
	else
	{
		temp->wsaBuf.len = szRecvBytes;
		WSASend(hRecvSocket, &(temp->wsaBuf), 1, &sentByte, 0, lpOverlapped, WriteCompRoutine);
	}
	
}

void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{

	LP_PER_IO_DATA temp = (LP_PER_IO_DATA)lpOverlapped->hEvent;
	SOCKET hRecvSocket = temp->recvSock;

	temp->wsaBuf.buf = temp->buf;
	temp->wsaBuf.len = BUF_SIZE;

	DWORD readByte = 0;
	DWORD dwFlag = 0;

	WSARecv(hRecvSocket, &(temp->wsaBuf), 1, &readByte, &dwFlag, lpOverlapped, ReadCompRoutine);

}