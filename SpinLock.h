#ifndef PCH_H
#define PCH_H

#include "pch.h"

#ifndef DLL_PROC
#define DLL_PROC extern "C" __declspec(dllexport)
#endif


DLL_PROC void __stdcall AcquireSpinLock(volatile long* lock);
DLL_PROC void __stdcall ReleaseSpinLock(volatile long* lock);

#endif // PCH_H