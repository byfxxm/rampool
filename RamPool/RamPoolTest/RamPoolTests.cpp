#include "stdafx.h"
#include "../RamPool/RamPool.h"

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

	printf("time rate: %f\n", 1.0 * (_time2 - _time1) / (_time3 - _time2));
}

void RamPool_Test1()
{
	int _nSizes[1000];
	srand((unsigned)time(0));

	for (auto& _size : _nSizes)
		_size = rand() % 10240;

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

	auto MultiThreadRun = [&_nSizes](function<void(int)> f_)
	{
		thread _ths[100];

		for (int _i = 0; _i < _countof(_ths); _i++)
		{
			thread _th([&]()
			{
				for (int _j = 0; _j < _countof(_nSizes); _j++)
					f_(_nSizes[_j]);
			});

			_ths[_i].swap(_th);
		}

		for (auto& _th : _ths)
		{
			_th.join();
		}
	};

	RamPool_Compare(100, [&]()
	{
		MultiThreadRun(RunRamPool);
	}, [&]()
	{
		MultiThreadRun(RunMMU);
	});

	rp_clear();
}
