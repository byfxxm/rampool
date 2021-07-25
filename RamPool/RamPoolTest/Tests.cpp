#include "stdafx.h"
#include "../RamPool/RamPool.h"

using namespace std;

static void RamPool_Compare(const char* test_, int times_, function<void()>f1_, function<void()>f2_)
{
	auto _time1 = clock();

	for (int _i = 0; _i < times_; _i++)
		f1_();

	auto _time2 = clock();

	for (int _i = 0; _i < times_; _i++)
		f2_();

	auto _time3 = clock();

	printf("%s time rate: %f\n", test_, 1.0 * (_time2 - _time1) / (_time3 - _time2));
}

void RamPool_Test1()
{
	int _nSizes[3000];
	srand((unsigned)time(0));

	for (auto& _size : _nSizes)
	{
		_size = rand() % 1024;
	}

	RamPool_Compare("Test1", 1000, [&]()
	{
		char** _p = (char**)rp_malloc(_countof(_nSizes) * sizeof(char*));

		for (int _i = 0; _i < _countof(_nSizes); _i++)
		{
			_p[_i] = (char*)rp_malloc(_nSizes[_i]);
		}

		for (int _i = 0; _i < _countof(_nSizes); _i++)
		{
			rp_free(_p[_i]);
		}

		rp_free(_p);
	}, [&]()
	{
		char** _p = (char**)malloc(_countof(_nSizes) * sizeof(char*));

		for (int _i = 0; _i < _countof(_nSizes); _i++)
		{
			_p[_i] = (char*)malloc(_nSizes[_i]);
		}

		for (int _i = 0; _i < _countof(_nSizes); _i++)
		{
			free(_p[_i]);
		}
		free(_p);
	});
}