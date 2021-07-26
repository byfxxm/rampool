#include "stdafx.h"
#include "../RamPool/RamPool.h"

#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif

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
	int _nSizes[100000];
	srand((unsigned)time(0));

	for (auto& _size : _nSizes)
		_size = rand() % 10240;

	auto RunRamPool = [](int size_)
	{
		void* _p = RamPool_Malloc(size_);
		RamPool_Free(_p);
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

	//rp_clear();
}

void RamPool_Test2()
{
	printf("test2\n");

	for (int _i = 0; _i < 100000; _i++)
	{
		RamPool_Malloc(1000);
	}

	RamPool_Destroy();
}

void RamPool_Test3()
{
	void* p1 = RamPool_Malloc(1000);
	void* p2 = RamPool_Malloc(1000);
	void* p3 = RamPool_Malloc(1000);

	RamPool_Free(p1);
	RamPool_Free(p1);
	RamPool_Free(p2);
	RamPool_Free(p2);
	RamPool_Free(p2);
	RamPool_Free(p3);
	RamPool_Free(p1);
}