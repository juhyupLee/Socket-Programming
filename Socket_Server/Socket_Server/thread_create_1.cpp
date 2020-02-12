#include "thread_create_1.h"

#include <process.h>
#include <Windows.h>

#include <stdio.h>
unsigned int WINAPI thread1(void* argc);

int Thread_Create_1(int argc, char* argv[])
{
	HANDLE hThread;
	unsigned threadID;


	int param = 5;

	hThread =(HANDLE) _beginthreadex(nullptr, 0, thread1, (void*)&param, 0, (unsigned int*)&threadID);

	if (hThread == 0)
	{
		puts("_beginthreadex error");
		return -1;

	}

	WaitForSingleObject(hThread, INFINITE);

	puts("Enf of Main");

	return 0;

}

unsigned int WINAPI thread1(void* argc)
{
	int count = *((int*)argc);

	for (int i = 0; i < count; ++i)
	{
		puts("Running Thread 1");
		Sleep(1000);
	}

	return 0;
}
