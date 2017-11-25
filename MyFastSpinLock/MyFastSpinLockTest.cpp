// MyFastSpinLock.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

unsigned WINAPI TestThread(LPVOID arg);

volatile unsigned long gSum = 0;
MyFastSpinLock gMyLock;

int main()
{
	unsigned int threadId;
	HANDLE thread1, thread2;

	thread1 = (HANDLE)_beginthreadex(NULL, 0, TestThread, NULL, 0, &threadId);
	thread2 = (HANDLE)_beginthreadex(NULL, 0, TestThread, NULL, 0, &threadId);

	WaitForSingleObject(thread1, INFINITE);
	WaitForSingleObject(thread2, INFINITE);

	printf_s("gSum: %d\n", gSum);

	CloseHandle(thread1);
	CloseHandle(thread2);

    return 0;
}

unsigned WINAPI TestThread(LPVOID arg)
{
	printf_s("Start Thread\n");

	for (int i = 0; i < 100; i++)
	{
		{
			MyFastSpinLockGuard criticalSection(gMyLock, false);
			gSum++;
			printf_s("Tick\t");
		}
	}

	printf_s("End Trhead\n");

	return 0;
}
