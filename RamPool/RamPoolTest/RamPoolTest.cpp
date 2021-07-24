#include "stdafx.h"
#include "../RamPool/RamPool.h"

int main()
{
	while (true)
	{
		void* _pRamPool = RamPool_Create();

		char* _pStr[3000];
		for (int _i = 0; _i < _countof(_pStr); _i++)
		{
			_pStr[_i] = (char*)RamPool_Malloc(_pRamPool, 17);
			_pStr[_i] = (char*)RamPool_Malloc(_pRamPool, 50);
		}

		RamPool_Delete(_pRamPool);
	}
}
