#include "pch.h"

#include <windows.h>

void AcquireSpinLock(volatile LONG* lock)
{
	while (_InterlockedExchange(lock, 1) == 1)
	{
		for (int i = 0; i < 1024; ++i) {
			YieldProcessor();
		}
	}
}

void ReleaseSpinLock(volatile LONG* lock)
{
	_InterlockedExchange(lock, 0);
}

