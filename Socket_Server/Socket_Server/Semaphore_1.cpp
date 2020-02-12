#include "Semaphore_1.h"
#include <Windows.h>
#include <process.h>
#include <iostream>

using namespace std;

 HANDLE hSemaphore1;
 HANDLE hSemaphore2;

int num = 0;
int sum3 = 0;
unsigned WINAPI Thread_Read_3(void* argc);
unsigned WINAPI Thread_Write_3(void* argc);

int Semaphroe_1(int argc, char* argv[])
{

	hSemaphore1 = CreateSemaphore(NULL, 0, 1, NULL);
	hSemaphore2 = CreateSemaphore(NULL, 1, 1, NULL);

	HANDLE hThread1;
	HANDLE hThread2;

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Thread_Read_3, NULL, NULL, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Thread_Write_3, NULL, NULL, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);


	cout << "Sum:" << sum3 << endl;

	CloseHandle(hSemaphore1);
	CloseHandle(hSemaphore2);

	return 0;
}

unsigned WINAPI Thread_Read_3(void* argc)
{
	
	for (int i = 0; i < 5; ++i)
	{
		WaitForSingleObject(hSemaphore2, INFINITE);
		cout << "Num:";
		cin >> num;
		ReleaseSemaphore(hSemaphore1, 1, NULL);
	}

	return 0;

}
unsigned WINAPI Thread_Write_3(void* argc)
{

	for (int i = 0; i < 5; ++i)
	{
		WaitForSingleObject(hSemaphore1, INFINITE);
		sum3 += num;
		ReleaseSemaphore(hSemaphore2, 0, NULL);

	}

	return 0;
}