#include "stdafx.h"
#include "../rampool/rampool.h"
#include "../lua/lua52/lua.hpp"

void* LuaAlloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
	if (nsize == 0) {
		rp_free(ptr);
		return nullptr;
	} else {
		return rp_realloc(ptr, nsize);
	}
}

void Leak(void* heap)
{
	LeakInfo info;
	heap ? rp_heap_leak(heap, &info) : rp_leak(&info);
	std::cout << std::dec << "leak count = " << info.count << ", leak size = " << info.total_size << ", leak actual size = " << info.total_actual_size << std::endl;
}

void RampoolCompare(int times, std::function<void()>f1, std::function<void()>f2)
{
	auto time1 = std::chrono::system_clock::now();
	for (int _i = 0; _i < times; _i++)
		f1();
	auto time2 = std::chrono::system_clock::now();
	for (int _i = 0; _i < times; _i++)
		f2();
	auto time3 = std::chrono::system_clock::now();

	printf("time ratio: %f\n", 1.0 * std::chrono::duration<double>(time2 - time1).count() / std::chrono::duration<double>(time3 - time2).count());
}

void Test1()
{
	int sizes[16000];
	srand((unsigned)time(0));

	for (auto& size : sizes)
		size = rand() % 10240 + 1;

	auto run_rampool = [](int size) {
		void* _p = rp_malloc(size);
		rp_free(_p);
	};
	
	auto run_mmu = [](int size) {
		void* _p = malloc(size);
		free(_p);
	};

	auto multi_thread_run = [&](std::function<void(int)> run)
	{
		std::thread thds[100];

		for (int _i = 0; _i < _countof(thds); _i++) {
			thds[_i] = std::thread([&]() {
				for (int _j = 0; _j < _countof(sizes); _j++)
					run(sizes[_j]);
				});
		}

		for (auto& th : thds)
			th.join();
	};

	RampoolCompare(1,
		[&]() {
			multi_thread_run(run_rampool);
		},
		[&]() {
			multi_thread_run(run_mmu);
		});
}

void Test2() {
	{
		std::shared_ptr<void> ps[16000];
		for (auto& pi : ps)
			pi.reset(rp_malloc(1000), rp_free);
	}

	Leak(nullptr);
}

void Test3() {
	void* p1 = rp_malloc(1001);
	void* p2 = rp_malloc(40);
	void* p3 = rp_malloc(11000);
	void* p4 = rp_malloc(1234);

	rp_free(p3);
	rp_free(p1);
	Leak(nullptr);
	rp_destroy();
}

void Test4() {
	auto lua = lua_newstate(LuaAlloc, nullptr);
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
		end")) {
		auto _s = lua_tostring(sub_lua_1, -1);
		std::cout << _s << std::endl;
	}

	luaL_dostring(sub_lua_2, "F2()");
	lua_close(lua);

	Leak(nullptr);
}

void Test5() {
	for (int _i = 0; _i < 100000; _i++)
		rp_malloc(_i % 1000 + 1);

	rp_destroy();
}

void Test6() {
	void* ps[100000];
	for (auto& pi : ps)
		pi = rp_malloc(20);
	for (auto& pi : ps)
		rp_free(pi);
	rp_gc();
}

void Test7() {
	__try {
		int n = 0;
		rp_free(&n);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		std::cout << std::hex << GetExceptionCode() << std::endl;
	}
}

void Test8() {
	std::atomic<void*> p[100];
	std::thread thd[_countof(p)];

	for (int i = 0; i < _countof(p); ++i) {
		thd[i] = std::thread([&, i]() {
			for (int j = 0; j < 100000; ++j)
				p[i] = rp_realloc(p[i], 18 + i % 8);
			});
	}

	for (auto& t : thd)
		t.join();
	for (auto& i : p)
		rp_free(i);

	Leak(nullptr);
}