// MyFastSpinLock.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

	for (int i = 0; i < 1000000; i++)
	{
		{
			MyFastSpinLockGuard criticalSection(gMyLock, true);
			//gMyLock.EnterWriteLock();
			gSum++;
			//gMyLock.LeaveWriteLock();
		}
	}

	printf_s("End Thread\n");

	return 0;
}

