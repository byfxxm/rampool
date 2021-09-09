#include "stdafx.h"
#include "../RamPool/RamPool.h"
#include "../Lua/Lua52/lua.hpp"

using namespace std;

void RamPool_Compare(int times_, function<void()>f1_, function<void()>f2_)
{
	auto _time1 = clock();

	for (int _i = 0; _i < times_; _i++)
		f1_();

	auto _time2 = clock();

	for (int _i = 0; _i < times_; _i++)
		f2_();

	auto _time3 = clock();

	printf("time radio: %f\n", 1.0 * (_time2 - _time1) / (_time3 - _time2));
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

	printf("leak size = %u\n", rp_leak());
	rp_destroy();
}

void RamPool_Test3()
{
	void* p1 = rp_malloc(1000);
	void* p2 = rp_malloc(1000);
	void* p3 = rp_malloc(1000);

	rp_free(p1);
	rp_free(p1);
	rp_free(p2);
	rp_free(p2);
	rp_free(p2);
	rp_free(p3);
	rp_free(p1);

	printf("leak size = %u\n", rp_leak());
}

void RamPool_Test4()
{
	auto _pLua = luaL_newstate();
	luaL_openlibs(_pLua);

	auto _pSubLua1 = lua_newthread(_pLua);
	auto _pSubLua2 = lua_newthread(_pLua);

	luaL_dostring(_pSubLua1, "function F1()"\
		"print('hello')"\
		"end");

	luaL_dostring(_pSubLua2, "F1()");
	//lua_close(_pLua);

	printf("leak size = %u\n", rp_leak());
}