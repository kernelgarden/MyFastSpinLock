#pragma once
#include <Windows.h>
#include <winnt.h>

#define MAX_WAIT_TIME 4091

class MyFastSpinLock
{
public:
	MyFastSpinLock();

	~MyFastSpinLock();

	void EnterReadLock();
	void LeaveReadLock();

	void EnterWriteLock();
	void LeaveWriteLock();

	unsigned long GetLockFlag() const { return mLockFlag; }

private:

	enum LockType
	{
		LT_READ_FLAG = 0x1,
		LT_READ_MASK = 0xFFFF,
		LT_WRITE_FLAG = 0x10000,
		LT_WRITE_MASK = 0xFFFF0000,
	};

	MyFastSpinLock(const MyFastSpinLock& rhs);
	MyFastSpinLock& operator=(const MyFastSpinLock& rhs);

	volatile long mLockFlag;
};

class MyFastSpinLockGuard
{
public:
	MyFastSpinLockGuard(MyFastSpinLock& lock, bool isExclusive);
	~MyFastSpinLockGuard();

private:
	MyFastSpinLock& mLock;
	bool isExclusive;
};

