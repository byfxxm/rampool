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
	LeakInfo _info;
	pRamPool_ ? RamPool_Leak(pRamPool_, &_info) : rp_leak(&_info);
	printf("leak count = %u, leak size = %u, leak actual size = %u\n", _info.nCount, _info.nTotalSize, _info.nTotalActualSize);
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
	int _nSizes[16000];
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
			_ths[_i] = thread([&]()
			{
				for (int _j = 0; _j < _countof(_nSizes); _j++)
					fRun_(_nSizes[_j]);
			});
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
		shared_ptr<void> _p[16000];
		for (auto& _pi : _p)
		{
			_pi.reset(rp_malloc(1000), rp_free);
		}
	}

	Leak(nullptr);
	//rp_destroy();
}

void RamPool_Test3()
{
	void* p1 = rp_malloc(1001);
	void* p2 = rp_malloc(40);
	void* p3 = rp_malloc(11000);
	void* p4 = rp_malloc(1234);

	rp_free(p3);
	rp_free(p1);
	rp_free(p2);
	rp_free(p4);
	Leak(nullptr);
	//rp_destroy();
}

void RamPool_Test4()
{
	auto _pLua = lua_newstate(LuaAlloc, nullptr);
	//auto _pLua = luaL_newstate();
	luaL_openlibs(_pLua);

	auto _pSubLua1 = lua_newthread(_pLua);
	auto _pSubLua2 = lua_newthread(_pLua);

	luaL_dostring(_pSubLua1, "function F1()\n"\
		"print('hello')\n"\
		"print(debug.traceback('world'))\n"\
		"end\n"\
		"function F2()\n"\
		"F1()\n"\
		"end");

	luaL_dostring(_pSubLua2, "F2()");
	lua_close(_pLua);

	Leak(nullptr);
}

void RamPool_Test5()
{
	void* _p[16000];

	for (int _i = 0; _i < _countof(_p); _i++)
	{
		_p[_i] = rp_malloc(20);
	}

	rp_destroy();
	Leak(nullptr);
}