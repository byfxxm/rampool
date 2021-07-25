#include "stdafx.h"
#include "../RamPool/RamPool.h"

using namespace std;

static void RamPool_Compare(int times_, function<void()>f1_, function<void()>f2_)
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
	int _nSizes[3000];
	srand((unsigned)time(0));

	for (auto& _size : _nSizes)
	{
		_size = rand() % 1024 + 1;
	}
	
	void* _pPool = RamPool_Create();

	RamPool_Compare(1000, [&]
	{
		char** _p = (char**)RamPool_Malloc(_pPool, _countof(_nSizes) * sizeof(char*));

		for (int _i = 0; _i < _countof(_nSizes); _i++)
			_p[_i] = (char*)RamPool_Malloc(_pPool, _nSizes[_i]);

		for (int _i = 0; _i < _countof(_nSizes); _i++)
			RamPool_Free(_pPool, _p[_i]);

		RamPool_Free(_pPool, _p);
	}, [&]
	{
		char** _p = (char**)malloc(_countof(_nSizes) * sizeof(char*));

		for (int _i = 0; _i < _countof(_nSizes); _i++)
			_p[_i] = (char*)malloc(_nSizes[_i]);

		for (int _i = 0; _i < _countof(_nSizes); _i++)
			free(_p[_i]);

		free(_p);
	});

	RamPool_Delete(_pPool);
}

void RamPool_Test2()
{
	thread* _pTh[5];

	for (auto& _p : _pTh)
	{
		_p = new thread(RamPool_Test1);
	}

	for (auto _p : _pTh)
	{
		_p->join();
		delete _p;
	}
}

void RamPool_Test3()
{
	condition_variable _cond;
	mutex _mut;

	void* _pPool = RamPool_Create();

	void* _p[64] = { 0 };
	HANDLE _hThMalloc[64];
	HANDLE _hThFree[64];

	while (1)
	{
		for (int _i = 0; _i < _countof(_p); _i++)
		{
			thread _th([&](int _index)
			{
				_p[_index] = RamPool_Malloc(_pPool, 50);
			}, _i);

			_hThMalloc[_i] = _th.native_handle();
			_th.detach();
		}

		for (volatile int _i = 0; _i < _countof(_p); _i++)
		{
			thread _th([&](int _index)
			{
				while (_p[_index] == nullptr);
				RamPool_Free(_pPool, (void*)_p[_index]);
			}, _i);

			_hThFree[_i] = _th.native_handle();
			_th.detach();
		}

		WaitForMultipleObjects(_countof(_hThMalloc), _hThMalloc, TRUE, INFINITE);
		WaitForMultipleObjects(_countof(_hThFree), _hThFree, TRUE, INFINITE);

		printf("$");
	}

	RamPool_Delete(_pPool);
}