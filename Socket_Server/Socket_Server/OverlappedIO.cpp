#include "OverlappedIO.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;

int OverlappedIO(int argc, char* argv[])
{
	WSAData wsaData;
	SOCKADDR_IN sockAddr;
	SOCKET servSock;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == -1)
	{
		cout << "WSAStart Up Fail" << endl;
		return -1;
	}


	//servSock =WSASocket(AF_INET, 0, ? , ? , g ? , dwFlags);

	sockAddr.sin_addr.s


	return 0;
}
