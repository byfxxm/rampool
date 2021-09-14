#include "stdafx.h"
#include "../RamPool/RamPool.h"

void RamPool_Test1();
void RamPool_Test2();
void RamPool_Test3();
void RamPool_Test4();
//void ReplaceIATEntryInOneMod(PCSTR callee_, PROC pfOld_, PROC pfNew_, PCSTR caller_);

int main()
{
	do
	{
		RamPool_Test1();
		RamPool_Test2();
		RamPool_Test3();
		RamPool_Test4();
		//ReplaceIATEntryInOneMod("Everything.exe");
		rp_destroy();
	} while (true);

	return 0;
}