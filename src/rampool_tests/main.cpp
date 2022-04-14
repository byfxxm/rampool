#include "stdafx.h"
#include "../rampool/rampool.h"

void Test1();
void Test2();
void Test3();
void Test4();
void Test5();
void Test6();
void Test7();
void Test8();

int main()
{
	do
	{
		rp_auto_gc(true);
		Test1();
		Test2();
		Test3();
		Test4();
		Test5();
		Test6();
		Test7();
		Test8();
	} while (1);

	return 0;
}