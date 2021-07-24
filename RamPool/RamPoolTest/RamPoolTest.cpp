#include "stdafx.h"
#include "../RamPool/RamPool.h"

int main()
{
	void* _pRamPool = RamPool_Create();

	while (true)
	{
		char* _pStr[300];
		for (int _i = 0; _i < _countof(_pStr); _i++)
		{
			_pStr[_i] = (char*)RamPool_Malloc(_pRamPool, 17);
		}
		//char* _p = (char*)RamPool_Malloc(_pRamPool, 100);

		//sprintf_s(_p, 100, "hello\n");

		//printf(_p);

		for (auto _p : _pStr)
		{
			RamPool_Free(_pRamPool, _p);
		}
	}

	RamPool_Delete(_pRamPool);
}
