// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "RamPool.h"

_declspec(thread) void* g_pRamPool = nullptr;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
		g_pRamPool = RamPool_Create();
		break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		RamPool_Delete(g_pRamPool);
        break;
    }
    return TRUE;
}

