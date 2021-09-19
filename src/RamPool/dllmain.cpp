#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule_, DWORD  dwReason_, LPVOID lpReserved_)
{
	switch (dwReason_)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}