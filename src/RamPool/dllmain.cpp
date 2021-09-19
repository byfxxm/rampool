#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule_, DWORD  ul_reason_for_call_, LPVOID lpReserved_)
{
	switch (ul_reason_for_call_)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}