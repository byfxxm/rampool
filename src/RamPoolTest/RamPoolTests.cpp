#include "stdafx.h"
#include "../RamPool/RamPool.h"
#include "../Lua/Lua52/lua.hpp"

void* LuaAlloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
	if (nsize == 0) {
		rp_free(ptr);
		return NULL;
	}
	else
	{
		return rp_realloc(ptr, nsize);
	}
}

void Leak(void* pRamPool_)
{
	size_t _count = 0;
	size_t _leaksize = 0;
	pRamPool_ ? RamPool_Leak(pRamPool_, &_count, &_leaksize) : rp_leak(&_count, &_leaksize);
	printf("leak count = %u, leak size = %u\n", _count, _leaksize);
}

void RamPool_Compare(int times_, function<void()>f1_, function<void()>f2_)
{
	auto _time1 = clock();

	for (int _i = 0; _i < times_; _i++)
		f1_();

	auto _time2 = clock();

	for (int _i = 0; _i < times_; _i++)
		f2_();

	auto _time3 = clock();

	printf("time radio: %f\n", 1.0 * ((double)_time2 - _time1) / ((double)_time3 - _time2));
}

void RamPool_Test1()
{
	int _nSizes[10000];
	srand((unsigned)time(0));

	for (auto& _size : _nSizes)
		_size = rand() % 10240 + 1;

	auto RunRamPool = [](int size_)
	{
		void* _p = rp_malloc(size_);
		rp_free(_p);
	};
	
	auto RunMMU = [](int size_)
	{
		void* _p = malloc(size_);
		free(_p);
	};

	auto MultiThreadRun = [&_nSizes](function<void(int)> fRun_)
	{
		thread _ths[100];

		for (int _i = 0; _i < _countof(_ths); _i++)
		{
			thread _th([&]()
			{
				for (int _j = 0; _j < _countof(_nSizes); _j++)
					fRun_(_nSizes[_j]);
			});

			_ths[_i].swap(_th);
		}

		for (auto& _th : _ths)
		{
			_th.join();
		}
	};

	RamPool_Compare(1, [&]()
	{
		MultiThreadRun(RunRamPool);
	}, [&]()
	{
		MultiThreadRun(RunMMU);
	});
}

void RamPool_Test2()
{
	{
		shared_ptr<void> _p[100000];
		for (int _i = 0; _i < _countof(_p); _i++)
		{
			_p[_i].reset(rp_malloc(1000), rp_free);
		}
	}

	Leak(nullptr);
	rp_destroy();
}

void RamPool_Test3()
{
	void* p1 = rp_malloc(1001);
	void* p2 = rp_malloc(1000);
	void* p3 = rp_malloc(1000);
	void* p4 = malloc(100);

	cout << rp_size(p4) << endl;
	cout << rp_size(p1) << endl;
	rp_free(p1);
	cout << rp_size(p1) << endl;
	rp_free(p2);
	rp_free(p3);
	free(p4);

	Leak(nullptr);
}

void RamPool_Test4()
{
	auto _pLua = lua_newstate(LuaAlloc, nullptr);
	luaL_openlibs(_pLua);

	auto _pSubLua1 = lua_newthread(_pLua);
	auto _pSubLua2 = lua_newthread(_pLua);

	luaL_dostring(_pSubLua1, "function F1()\n"\
		"print('hello')\n"\
		"print(debug.traceback('world'))\n"\
		"end");

	luaL_dostring(_pSubLua2, "F1()");
	lua_close(_pLua);

	Leak(nullptr);
}

void ReplaceIATEntryInOneMod(PCSTR callee_, PROC pfOld_, PROC pfNew_, PCSTR caller_)
{
	HMODULE hmodCaller_ = GetModuleHandle(caller_);

	ULONG ulSize;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;
	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hmodCaller_, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

	if (!pImportDesc)
		return;

	for (; pImportDesc->Name; pImportDesc++)
	{
		PSTR pszModName = (PSTR)((PBYTE)hmodCaller_ + pImportDesc->Name);
		if (lstrcmpi(pszModName, callee_) == 0)
		{
			PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((PBYTE)hmodCaller_ + pImportDesc->FirstThunk);
			for (; pThunk->u1.Function; pThunk++)
			{
				PROC* ppfn = (PROC*)&pThunk->u1.Function;
				BOOL bFound = (*ppfn == pfOld_);
				if (bFound)
				{
					if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfNew_, sizeof(pfNew_), NULL)
						&& (ERROR_NOACCESS == GetLastError()))
					{
						DWORD dwOldProtect;
						if (VirtualProtect(ppfn, sizeof(*ppfn), PAGE_WRITECOPY, &dwOldProtect))
						{
							WriteProcessMemory(GetCurrentProcess(), ppfn, &pfNew_, sizeof(pfNew_), NULL);
							VirtualProtect(ppfn, sizeof(pfNew_), dwOldProtect, &dwOldProtect);
						}
					}
					return;
				}
			}
		}
	}
}