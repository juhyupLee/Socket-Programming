#include "OutOfBand_Send.h"
#include <WinSock2.h>
#include <iostream>
#include "interface_Func.h"

void OOB_Send(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET servSocket;
	
	if (WSAStartup(MAKEWORD(2, 2),&wsaData) == -1)
	{
		ErrorMessage("WSAStartup Error");
	}

	servSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (servSocket == INVALID_SOCKET)
	{
		ErrorMessage("Socket Error");
	}

	//connect();


}
