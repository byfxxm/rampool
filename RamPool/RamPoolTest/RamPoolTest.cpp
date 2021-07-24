#include "stdafx.h"
#include "../RamPool/RamPool.h"

int main()
{
	int _times[30000];

	OutputDebugString("begin1\n");
	for (auto _i : _times)
	{
		void* _pRamPool = RamPool_Create();

		char* _pStr[3000];
		char* _pStr1[3000];
		for (int _i = 0; _i < _countof(_pStr); _i++)
		{
			_pStr[_i] = (char*)RamPool_Malloc(_pRamPool, 17);
			_pStr1[_i] = (char*)RamPool_Malloc(_pRamPool, 50);
		}

		//for (int _i = 0; _i < _countof(_pStr); _i++)
		//{
		//	RamPool_Free(_pRamPool, _pStr1[_i]);
		//	RamPool_Free(_pRamPool, _pStr[_i]);
		//}

		RamPool_Delete(_pRamPool);
	}
	OutputDebugString("end1\n");

	OutputDebugString("begin2\n");
	for (auto _i : _times)
	{
		char* _pStr[3000];
		char* _pStr1[3000];
		for (int _i = 0; _i < _countof(_pStr); _i++)
		{
			_pStr[_i] = (char*)malloc(17);
			_pStr1[_i] = (char*)malloc(50);
		}

		for (int _i = 0; _i < _countof(_pStr); _i++)
		{
			free(_pStr1[_i]);
			free(_pStr[_i]);
		}
	}
	OutputDebugString("end2\n");
}
