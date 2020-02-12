#include "CriticalSection_1.h"
#include <process.h>
#include <windows.h>
#include <iostream>
using namespace std;

unsigned WINAPI Thread_INC(void* argv);
unsigned WINAPI Thread_DEC(void* argv);

const int threadCnt = 10;
int sum = 0;

CRITICAL_SECTION criticalSection;

int Critical_Section_1(int argc, char* argv[])
{
	HANDLE threadHandle[threadCnt] = { 0 };

	unsigned int thredID[threadCnt] = { 0 };

	InitializeCriticalSection(&criticalSection);

	for (int i = 0; i < threadCnt; ++i)
	{
		if (i % 2)
		{
			_beginthreadex(nullptr, 0, Thread_INC, nullptr, 0, &thredID[i]);
		}
		else
		{
			_beginthreadex(nullptr, 0, Thread_DEC, nullptr, 0, &thredID[i]);
		}

	}

	WaitForMultipleObjects(threadCnt, threadHandle, 1, INFINITE);

	DeleteCriticalSection(&criticalSection);

	cout << "결과는 :" << sum << endl;

	cout << "Main문 종료" << endl;
	return 0;
}
unsigned WINAPI Thread_INC(void* argc)
{

	EnterCriticalSection(&criticalSection);
	for (int i = 0; i < 500000; ++i)
	{
		sum+=i;
	}
	LeaveCriticalSection(&criticalSection);

	return 1;

}
unsigned WINAPI Thread_DEC(void* argc)
{

	EnterCriticalSection(&criticalSection);
	for (int i = 0; i < 500000; ++i)
	{
		sum -= i;
	}
	LeaveCriticalSection(&criticalSection);

	return 1;
}
