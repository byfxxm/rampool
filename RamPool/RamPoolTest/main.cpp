#include "stdafx.h"
#include "../RamPool/RamPool.h"

void RamPool_Test1();
void RamPool_Test2();

int main()
{
	void* p1 = rp_malloc(1000);
	void* p2 = rp_malloc(1000);
	void* p3 = rp_malloc(1000);

	rp_free(p1);
	rp_free(p1);
	rp_free(p2);
	rp_free(p2);
	rp_free(p2);
	rp_free(p3);
	rp_free(p1);

	while (true)
	{
		RamPool_Test1();
		//RamPool_Test2();
		//break;
	}
}
