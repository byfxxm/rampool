#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule_, DWORD  nReason_, LPVOID lpReserved_)
{
	switch (nReason_)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}