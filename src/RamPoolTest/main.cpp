#include "stdafx.h"
#include "../RamPool/RamPool.h"

void RamPool_Test1();
void RamPool_Test2();
void RamPool_Test3();
void RamPool_Test4();
void RamPool_Test5();

int main()
{
	do
	{
		rp_auto_gc(true);
		RamPool_Test1();
		RamPool_Test2();
		RamPool_Test3();
		RamPool_Test4();
		RamPool_Test5();
	} while (true);

	return 0;
}