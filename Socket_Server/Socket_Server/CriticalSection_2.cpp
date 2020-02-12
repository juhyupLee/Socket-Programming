#include "CriticalSection_2.h"
#include <process.h>
#include <windows.h>
#include <iostream>
using namespace std;

unsigned WINAPI Thread_INC_2(void* argc);
unsigned WINAPI Thread_DEC_2(void* argc);
const int HANDLE_COUNT = 10;

int sum2 = 0;

HANDLE hMutex;

int Critical_Section_2(int argc, char* argv[])
{

	HANDLE handleArr[HANDLE_COUNT] = { 0 };

	hMutex = CreateMutex(nullptr, false, nullptr);

	for (int i = 0; i < HANDLE_COUNT; ++i)
	{
		if (i % 2)
		{
			_beginthreadex(nullptr, 0, Thread_INC_2, nullptr, 0, nullptr);
		}
		else
		{
			_beginthreadex(nullptr, 0, Thread_DEC_2, nullptr, 0, nullptr);
		}
	}

	WaitForMultipleObjects(HANDLE_COUNT, handleArr, true, INFINITE);


	CloseHandle(hMutex);

	cout << "Sum:" << sum2 << endl;


	return 0;
}

unsigned WINAPI Thread_INC_2(void* argc)
{
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < 500000; ++i)
	{
		sum2 += i;
	}
	ReleaseMutex(hMutex);

	return 0;
}
unsigned WINAPI Thread_DEC_2(void* argc)
{
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < 500000; ++i)
	{
		sum2 -= i;
	}
	ReleaseMutex(hMutex);

	return 0;
}
