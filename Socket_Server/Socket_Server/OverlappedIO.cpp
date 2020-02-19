#include "OverlappedIO.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;

int OverlappedIO(int argc, char* argv[])
{
	WSAData wsaData;
	SOCKADDR_IN sockAddr;
	SOCKET servSock,clntSock;
	WSABUF wsaBuf;

	char buf[1024];
	wsaBuf.buf = buf;
	wsaBuf.len = 1024;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == -1)
	{
		cout << "WSAStart Up Fail" << endl;
		return -1;
	}


	servSock =WSASocket(PF_INET, SOCK_STREAM, 0 , NULL , 0 ,WSA_FLAG_OVERLAPPED);
	if (servSock == INVALID_SOCKET)
	{
		cout << "Socket Fail" << endl;
		return -1;
	}

	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(atoi(argv[1]));
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(servSock, (sockaddr*)& sockAddr, sizeof(sockAddr)))
	{
		cout << "Bind Error" << endl;
		return -1;
	}

	if (listen(servSock, 5) == -1)
	{
		cout << "Listen Error" << endl;
		return -1;
	}
	int szClntAddr = 0;

	clntSock = accept(servSock, (SOCKADDR*)& sockAddr, &szClntAddr);

	DWORD recvByte = 0;
	DWORD flag = 0;
	DWORD sendByte = 0;

	WSAOVERLAPPED overlapped;

	WSAEVENT eventObj;

	eventObj = WSACreateEvent();

	overlapped.hEvent = eventObj;


	if (WSARecv(clntSock, &wsaBuf, 1, &recvByte, &flag, &overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			cout << "Background Data Recv...." << endl;
			WSAWaitForMultipleEvents(1, &eventObj, true, WSA_INFINITE, false);
			if (WSAGetOverlappedResult(clntSock, &overlapped, &recvByte, 0, NULL))
			{
				cout << "Recv Finish" << endl;
			}
		}
	}
	//WSASend에서 dwFlag는?
	//WSARecv에서는 flag변수의 주소를 넣어줫는데,,?
	if (WSASend(clntSock, &wsaBuf, 1, &sendByte, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			cout << "Background Date Send.." << endl;
			if (WSAGetOverlappedResult(clntSock, &overlapped, &sendByte, 0, NULL))
			{
				cout << "Send Finish" << endl;
			}
		}
	}

	closesocket(clntSock);
	closesocket(servSock);
	WSACloseEvent(eventObj);

	WSACleanup();


	return 0;
}
