#include "stdafx.h"
#include "../RamPool/RamPool.h"

int main()
{
	void* _pRamPool = RamPool_Create();

	void* _p = RamPool_Malloc(_pRamPool, 100);
	RamPool_Free(_pRamPool, _p);

	RamPool_Delete(_pRamPool);
}
