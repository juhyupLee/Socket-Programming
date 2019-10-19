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

	SOCKET servSock; //UDP는 연결의 개념이 아니기때문에 서버소켓만 있다.

	SOCKADDR_IN serverAddr, clntAddr;

	int clntAddrSize = 0;
	int strLen = 0;
	char message[eBUFSIZE];


	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		ErrorMessage("WStartup() Error");
		exit(1);

	}

	servSock = socket(PF_INET, SOCK_DGRAM, 0); // socket 매개변수에 대한 공부가 필요 
	if (servSock == INVALID_SOCKET)
	{
		ErrorMessage("socket() error)");
		exit(1);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET; // AF_INET에 대한 공부가 필요함
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //현재 호스트의 주소를 가져옴
	serverAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)& serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		ErrorMessage("bind() Error");
		exit(1);
	}

	//TCP ip 였다면 listen 만들고 connect 만들었을텐데 여기서는 아님)

	while (1)
	{
		clntAddrSize = sizeof(clntAddr);
		strLen = recvfrom(servSock, message, eBUFSIZE, 0, (SOCKADDR*)& clntAddr, &clntAddrSize);//recvfrom함수에서는 소켓을 넣어줄때, client 소켓이아니라, server의소켓을 넣어준다.
																							    //UDP  소켓은 TCP 소켓과달리 1개만 필요한데 그게 서버에서만든 소켓이다.
		sendto(servSock, message, strLen, 0, (SOCKADDR*)& clntAddr, sizeof(clntAddr));// 마찬가지로 snedto함수에서도 소켓을 넣어줄때 client소켓이아니라 server의 소켓으로 한다.
	}

	closesocket(servSock);
	WSACleanup();
	

}
