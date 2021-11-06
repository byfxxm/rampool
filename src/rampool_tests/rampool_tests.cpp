#include "stdafx.h"
#include "../rampool/rampool.h"
#include "../lua/lua52/lua.hpp"

#pragma comment(lib, "../release/libtcmalloc_minimal.lib")
#define DLL_IMPORT __declspec(dllimport)
extern "C"
{
	DLL_IMPORT void* tc_malloc(size_t);
	DLL_IMPORT void tc_free(void*);
}

void* lua_alloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
	if (nsize == 0) {
		rp_free(ptr);
		return nullptr;
	}
	else
	{
		return rp_realloc(ptr, nsize);
	}
}

void leak(void* heap)
{
	leak_info info;
	heap ? rp_heap_leak(heap, &info) : rp_leak(&info);
	printf("leak count = %u, leak size = %u, leak actual size = %u\n", info.count, info.total_size, info.total_actual_size);
}

void rampool_compare(int times, function<void()>f1, function<void()>f2)
{
	auto _time1 = clock();

	for (int _i = 0; _i < times; _i++)
		f1();

	auto _time2 = clock();

	for (int _i = 0; _i < times; _i++)
		f2();

	auto _time3 = clock();

	printf("time ratio: %f\n", 1.0 * ((double)_time2 - _time1) / ((double)_time3 - _time2));
}

void test1()
{
	int sizes[16000];
	srand((unsigned)time(0));

	for (auto& size : sizes)
		size = rand() % 10240 + 1;

	auto run_rampool = [](int size)
	{
		void* _p = rp_malloc(size);
		rp_free(_p);
	};
	
	auto run_mmu = [](int size)
	{
		void* _p = malloc(size);
		free(_p);
	};

	auto run_tcmalloc = [](int size)
	{
		void* _p = tc_malloc(size);
		tc_free(_p);
	};

	auto multi_thread_run = [&](function<void(int)> run)
	{
		thread thds[100];

		for (int _i = 0; _i < _countof(thds); _i++)
		{
			thds[_i] = thread([&]()
			{
				for (int _j = 0; _j < _countof(sizes); _j++)
					run(sizes[_j]);
			});
		}

		for (auto& th : thds)
		{
			th.join();
		}
	};

	rampool_compare(1, [&]()
		{
			multi_thread_run(run_rampool);
		}, [&]()
		{
			multi_thread_run(run_tcmalloc);
		});
}

void test2()
{
	{
		shared_ptr<void> ps[16000];
		for (auto& pi : ps)
		{
			pi.reset(rp_malloc(1000), rp_free);
		}
	}

	leak(nullptr);
}

void test3()
{
	void* p1 = rp_malloc(1001);
	void* p2 = rp_malloc(40);
	void* p3 = rp_malloc(11000);
	void* p4 = rp_malloc(1234);

	rp_free(p3);
	rp_free(p1);
	leak(nullptr);
	rp_destroy();
}

void test4()
{
	auto lua = lua_newstate(lua_alloc, nullptr);
	luaL_openlibs(lua);

	auto sub_lua_1 = lua_newthread(lua);
	auto sub_lua_2 = lua_newthread(lua);

	if (luaL_dostring(sub_lua_1,
		"function F1()\n\
			print('hello')\n\
			print(debug.traceback('world'))\n\
		end\n\
		tab = {'abc', 'haha', 9527}\n\
		function F2()\n\
			for i, v in ipairs(tab) do\n\
				print(v)\n\
			end\n\
			F1()\n\
		end"))
	{
		auto _s = lua_tostring(sub_lua_1, -1);
		cout << _s << endl;
	}

	luaL_dostring(sub_lua_2, "F2()");
	lua_close(lua);

	leak(nullptr);
}

void test5()
{
	for (int _i = 0; _i < 100000; _i++)
	{
		rp_malloc(_i % 1000 + 1);
	}

	rp_destroy();
}

void test6()
{
	void* ps[100000];

	for (auto& pi : ps)
	{
		pi = rp_malloc(20);
	}

	for (auto& pi : ps)
	{
		rp_free(pi);
	}

	rp_gc();
}

void test7()
{
	__try
	{
		int n = 0;
		rp_free(&n);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		cout << hex << GetExceptionCode() << endl;
	}
}

void test8()
{
	atomic<void*> p[100] = { 0 };
	thread thd[_countof(p)];

	for (int i = 0; i < _countof(p); ++i)
	{
		thd[i] = thread([&, i]()
			{
				for (int j = 0; j < 100000; ++j)
					p[i] = rp_realloc(p[i], 18 + i % 8);
			});
	}

	for (auto& t : thd)
		t.join();

	for (auto& i : p)
		rp_free(i);

	leak(nullptr);
}