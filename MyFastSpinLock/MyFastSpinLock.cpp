#include "stdafx.h"
#include "MyFastSpinLock.h"

MyFastSpinLock::MyFastSpinLock() :mLockFlag(0)
{ }

MyFastSpinLock::~MyFastSpinLock()
{ }

void MyFastSpinLock::EnterReadLock()
{
	while (true)
	{
		// Wait Write Lock
		while (mLockFlag & LT_WRITE_MASK)
			YieldProcessor();

		// InterlockedIncrement/InterlockedDecrement �迭�� �Լ��� ������ ������ ���� ��ȯ�Ѵ�.
		if ((InterlockedIncrement(&mLockFlag) & LT_WRITE_MASK) == 0)
			return;

		InterlockedDecrement(&mLockFlag);
	}
}

void MyFastSpinLock::LeaveReadLock()
{
	InterlockedDecrement(&mLockFlag);
}

void MyFastSpinLock::EnterWriteLock()
{
	while (true)
	{
		// Wait Write Lock
		while (mLockFlag & LT_WRITE_MASK)
			YieldProcessor();

		// InterlockedAdd �Լ��� ������ ���� �� ���� ��ȯ�Ѵ�.
		if ((InterlockedAdd(&mLockFlag, LT_WRITE_FLAG) & LT_WRITE_MASK) == LT_WRITE_FLAG)
		{
			// Wait Read Lock
			while (mLockFlag & LT_READ_MASK)
				YieldProcessor();

			return;
		}

		InterlockedAdd(&mLockFlag, -LT_WRITE_FLAG);
	}
}

void MyFastSpinLock::LeaveWriteLock()
{
	InterlockedAdd(&mLockFlag, -LT_WRITE_FLAG);
}

MyFastSpinLockGuard::MyFastSpinLockGuard(MyFastSpinLock& lock, bool isExclusive) :mLock(lock), isExclusive(isExclusive)
{ 
	if (isExclusive)
		mLock.EnterWriteLock();
	else
		mLock.EnterReadLock();
}

MyFastSpinLockGuard::~MyFastSpinLockGuard()
{ 
	if (isExclusive)
		mLock.LeaveWriteLock();
	else
		mLock.LeaveReadLock();
}