//#include "Multiplexing.h"
//#include <stdio.h>
//#include <string.h>
//#include <WinSock2.h>
//#include "interface_Func.h"
//#define BUF_SIZE 1024
//#include <iostream>
//using namespace std;
//
//int Multiplexing_1(int argc, char* argv[])
//{
//	WSAData wsaData;
//	SOCKET hServSock, hClntSock;
//	SOCKADDR_IN servAdr, clntAdr;
//	TIMEVAL timeout;
//	fd_set reads, cpyReads;
//
//	int adrSz;
//	int strLen, fdNum, i;
//	char buf[BUF_SIZE];
//
//	if (argc != 2)
//	{
//		cout << "Usage:" << argv[0] << "<port>" << endl;
//		exit(1);
//	}
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		ErrorMessage("wSAStartup() error!");
//	}
//	
//	hServSock = socket(PF_INET, SOCK_STREAM, 0);
//	memset(&servAdr, 0, sizeof(servAdr));
//	servAdr.sin_family = AF_INET;
//	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
//	servAdr.sin_port = htons(atoi(argv[1]));
//
//	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
//	{
//		ErrorMessage("bind() error");
//	}
//	if (listen(hServSock, 5) == SOCKET_ERROR)
//	{
//		ErrorMessage("listen() error");
//	}
//	
//	FD_ZERO(&reads);
//	FD_SET(hServSock, &reads);
//
//	while (1)
//	{
//		cpyReads = reads;
//		timeout.tv_sec = 5;
//		timeout.tv_usec = 5000;
//
//		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
//		{
//			break;
//		}
//
//		if (fdNum == 0)
//		{
//			continue;
//		}
//
//		for (i = 0; i < reads.fd_count; ++i)
//		{
//			if (FD_ISSET(reads.fd_array[i], &cpyReads))
//			{
//				if (reads.fd_array[i] == hServSock)
//				{
//					adrSz = sizeof(clntAdr);
//					hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
//					FD_SET(hClntSock, &reads);
//					printf("connected client : %d \n", hClntSock);
//				}
//				else
//				{
//					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
//					if (strLen == 0)
//					{
//						FD_CLR(reads.fd_array[i], &reads);
//						closesocket(cpyReads.fd_array[i]);
//						printf("closed client:%d\n", cpyReads.fd_array[i]);
//					}
//					else
//					{
//						send(reads.fd_array[i], buf, strLen, 0);
//					}
//
//
//				}
//			}
//		}
//
//
//	}
//
//	closesocket(hServSock);
//	WSACleanup();
//	return 0;
//}


#include <Winsock2.h>
#include <iostream>
#include "interface_Func.h"
#include "Multiplexing.h"
using namespace std;
#define BUF_SIZE 1024


int Multiplexing_1(int argc, char* argv[])
{
	WSADATA wsaData; 
	SOCKET hServSock, hClntSock;
	TIMEVAL timeout;
	FD_SET read;

	//SOCKADDR servAddr;
	//
	SOCKADDR_IN servAddr,clntAddr;
	int szAddr = 0;
	int strLen = 0;

	char buf[BUF_SIZE];

	if (argc != 2)
	{
		cout << "Port Error" << endl;
		return -1;
	}

	if (WSAStartup(MAKEWORD(2, 2),&wsaData) != 0)
	{
		cout << "WSAStartup Error" << endl;
		return -1;
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0); // PF INET vs AF_INET 먼차이? @@@: 틀림 (AF_INET으로씀)

	servAddr.sin_family = AF_INET; // 여기는 AF_INET이 맞네?
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY의 정확한 의미 식별필요
	servAddr.sin_port =htons( atoi(argv[1]));

	//szAddr = sizeof(servAddr);

	if (bind(hServSock, (sockaddr*)&servAddr, sizeof(servAddr)) != 0)
	{
		cout << "Bind Error" << endl;
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		cout << "Listen Error" << endl;
	}

	FD_ZERO(&read);
	FD_SET(hServSock, &read);

	FD_SET cpyRead;

	while (1)
	{
		cpyRead = read;

		timeout.tv_sec = 6;
		timeout.tv_usec = 500;

		int selectRtn = select(0, &cpyRead, nullptr, nullptr, &timeout); // 왜 fd_max가0이야?

		if(selectRtn ==SOCKET_ERROR)
		{
			cout << "오류 발생";
			return -1;
		}
		if (selectRtn == 0)// 타임아웃.
		{
			continue;
		}

		for (unsigned int i = 0; i < cpyRead.fd_count; ++i)
		{
			if (FD_ISSET(i, &cpyRead))// 파일디스크립터중에 셋된애 있니?
			{
				if (cpyRead.fd_array[i] == hServSock) //그 파일스크립터가 혹시 Server 소켓이니?
				{
					//그렇다면 accept로 받아서 클라이언트 소켓을 넣어야겟구나!
					szAddr = sizeof(clntAddr); // @@@이부분 틀림 

					hClntSock = accept(hServSock, (sockaddr*)&clntAddr, &szAddr);

					// accept 함수에서 addrlen이 왜필요할까?
					// 
					FD_SET(hClntSock,&read); 
					// 클라이언트의 소켓을 fd_set에 등록해버리자 : 파일디스크립터를 fd_set에 등록하자
					cout << "등록된 Client : " << hClntSock << endl;

				}
				else // 그 파일디스크립터가 혹시 Clnt소켓이니?
				{
					strLen = recv(cpyRead.fd_array[i], buf, BUF_SIZE-1, 0);
					//recv 함수는  잘썼는데,
					//recv안에 소켓 은 대상이다 클라이언트로부터 받아오는거니깐 클라이언트의 소켓을써야지

					
					
						//serv소켓이아니라 클라이언트에게 보낼것이니..
						//send 와 recv의 파일디스크립터는 어떻게되는거지?
						//서버입장에서 send와 recv의 파일디스크립터는  클라이언트 소켓이된다.

					
					//만약 받은 문자열의 길이가, 0이라면 소켓 연결을 종료했다는 뜻?
					if (strLen == 0)
					{
						cout << "삭제될 소켓: " << cpyRead.fd_array[i] << endl;
						FD_CLR(cpyRead.fd_array[i], &read);
						closesocket(cpyRead.fd_array[i]);
						
					}
					else
					{
						send(cpyRead.fd_array[i], buf, BUF_SIZE - 1, 0);// 에코다

					}
					
				}
			}
		}

	}

	closesocket(hServSock);
	WSACleanup();

	return 0;
}
