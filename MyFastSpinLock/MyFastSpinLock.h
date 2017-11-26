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
		LT_READ_MASK = 0xFFFFF,
		LT_WRITE_FLAG = 0x100000,
		LT_WRITE_MASK = 0x7FF00000,
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

