#include "stdafx.h"
#include "../rampool/rampool.h"

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();

int main()
{
	do
	{
		rp_auto_gc(true);
		test1();
		test2();
		test3();
		test4();
		test5();
		test6();
		test7();
		test8();
	} while (1);

	return 0;
}