#include "pch.h"


BOOL APIENTRY DllMain(	HMODULE hModule,
						DWORD ul_reason_for_call,
						LPVOID lpReserved)
{
	WCHAR wchBuffer[256];

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		swprintf_s(wchBuffer, L"Process attach: %d\n", GetCurrentProcessId());
		OutputDebugString(wchBuffer);
		break;
	case DLL_THREAD_ATTACH:
		swprintf_s(wchBuffer, L"Thread attach: %d\n", GetCurrentThreadId());
		OutputDebugString(wchBuffer);
		break;
	case DLL_THREAD_DETACH:
		swprintf_s(wchBuffer, L"Thread detach: %d\n", GetCurrentThreadId());
		OutputDebugString(wchBuffer);
		break;
	case DLL_PROCESS_DETACH:
		swprintf_s(wchBuffer, L"Process detach: %d\n", GetCurrentProcessId());
		OutputDebugString(wchBuffer);
		break;
	}
	return true;
}

