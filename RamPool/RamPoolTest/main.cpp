#include "stdafx.h"

using namespace std;
void RamPool_Test1();
void RamPool_Test2();
void RamPool_Test3();
void RamPool_Compare(int times_, function<void()>f1_, function<void()>f2_);
void RunRamPool();
void RunMMU();

int main()
{
	while (true)
	{
		//RamPool_Test1();
		//RamPool_Test2();
		//RamPool_Test3();
		RamPool_Compare(100, RunRamPool, RunMMU);
		//break;
	}
}
