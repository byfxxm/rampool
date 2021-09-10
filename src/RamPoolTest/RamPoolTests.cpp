#include "stdafx.h"
#include "../RamPool/RamPool.h"
#include "../Lua/Lua52/lua.hpp"

using namespace std;

void Leak(void* pRp_)
{
	size_t _count = 0;
	size_t _leaksize = 0;
	pRp_ ? RamPool_Leak(pRp_, &_count, &_leaksize) : rp_leak(&_count, &_leaksize);
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

	Leak(nullptr);
}

void RamPool_Test4()
{
	auto _pLua = luaL_newstate();
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

void RamPool_Test5()
{
	lua_State* L = luaL_newstate();    //初始化lua
	luaL_openlibs(L);    //载入所有lua标准库

	string s;
	while (getline(cin, s))    //从cin中读入一行到s
	{
		//载入s里的lua代码后执行
		bool err = luaL_loadbuffer(L, s.c_str(), s.length(),
			"line") || lua_pcall(L, 0, 0, 0);
		if (err)
		{
			//如果错误，显示
			cerr << lua_tostring(L, -1);
			//弹出错误信息所在的最上层栈
			lua_pop(L, 1);
		}
	}

	lua_close(L);//关闭
}