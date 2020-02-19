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

	//inet_pton 헤더파일 무엇? 왜 안되지?
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr.s_addr);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = atoi(argv[2]);

	//Connect의 소켓은 클라이언트? 서버?

	if (-1 == connect(hClntSock, (SOCKADDR*)&servAddr, sizeof(servAddr)))
	{
		cout << "Connect Fail" << endl;
		return -1;
	}


	//connect가 뭘 반환했지? 서버의 소켓을 반환했나?

	// 클라이언트는 소켓생성할떄, 필요가없구나?





	// 클라이언트에서 주소 바인딩을 했던가? -> 그런거같다 서버의 주소를 알아야 통신이되니깐
	//--> 아니다 클라이언트소켓은 주소와 바인딩하지 않는다. 누군가 클라이언트의 주소로 뭔가를 보내는것이아니기 때문이다
	// 클라이언트가 서버에 요청을하고, 서버는 그 요청한 클라이언트의 소켓만가지고 얼마든지 통신을 할 수 있기때문에 
	// 그치만 처음 connect가 되려면 서버의 주소를 알아야한다.


	// 했다면 왜 하지?
	// 서버의 주소를 알아야지
	// INADDR ANY?
	// 클라이언트 소켓은 언제생성?
	// 서버의 소켓은 서버쪽이 생성하고, 클라이언트의 소켓은 클라이언트가 생성


	return 0;
}
