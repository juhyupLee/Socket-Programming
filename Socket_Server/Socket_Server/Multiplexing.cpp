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

	hServSock = socket(PF_INET, SOCK_STREAM, 0); // PF INET vs AF_INET ������? @@@: Ʋ�� (AF_INET���ξ�)

	servAddr.sin_family = AF_INET; // ����� AF_INET�� �³�?
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY�� ��Ȯ�� �ǹ� �ĺ��ʿ�
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

		int selectRtn = select(0, &cpyRead, nullptr, nullptr, &timeout); // �� fd_max��0�̾�?

		if(selectRtn ==SOCKET_ERROR)
		{
			cout << "���� �߻�";
			return -1;
		}
		if (selectRtn == 0)// Ÿ�Ӿƿ�.
		{
			continue;
		}

		for (unsigned int i = 0; i < cpyRead.fd_count; ++i)
		{
			if (FD_ISSET(i, &cpyRead))// ���ϵ�ũ�����߿� �µȾ� �ִ�?
			{
				if (cpyRead.fd_array[i] == hServSock) //�� ���Ͻ�ũ���Ͱ� Ȥ�� Server �����̴�?
				{
					//�׷��ٸ� accept�� �޾Ƽ� Ŭ���̾�Ʈ ������ �־�߰ٱ���!
					szAddr = sizeof(clntAddr); // @@@�̺κ� Ʋ�� 

					hClntSock = accept(hServSock, (sockaddr*)&clntAddr, &szAddr);

					// accept �Լ����� addrlen�� ���ʿ��ұ�?
					// 
					FD_SET(hClntSock,&read); 
					// Ŭ���̾�Ʈ�� ������ fd_set�� ����ع����� : ���ϵ�ũ���͸� fd_set�� �������
					cout << "��ϵ� Client : " << hClntSock << endl;

				}
				else // �� ���ϵ�ũ���Ͱ� Ȥ�� Clnt�����̴�?
				{
					strLen = recv(cpyRead.fd_array[i], buf, BUF_SIZE-1, 0);
					//recv �Լ���  �߽�µ�,
					//recv�ȿ� ���� �� ����̴� Ŭ���̾�Ʈ�κ��� �޾ƿ��°Ŵϱ� Ŭ���̾�Ʈ�� �����������

					
					
						//serv�����̾ƴ϶� Ŭ���̾�Ʈ���� �������̴�..
						//send �� recv�� ���ϵ�ũ���ʹ� ��ԵǴ°���?
						//�������忡�� send�� recv�� ���ϵ�ũ���ʹ�  Ŭ���̾�Ʈ �����̵ȴ�.

					
					//���� ���� ���ڿ��� ���̰�, 0�̶�� ���� ������ �����ߴٴ� ��?
					if (strLen == 0)
					{
						cout << "������ ����: " << cpyRead.fd_array[i] << endl;
						FD_CLR(cpyRead.fd_array[i], &read);
						closesocket(cpyRead.fd_array[i]);
						
					}
					else
					{
						send(cpyRead.fd_array[i], buf, BUF_SIZE - 1, 0);// ���ڴ�

					}
					
				}
			}
		}

	}

	closesocket(hServSock);
	WSACleanup();

	return 0;
}
