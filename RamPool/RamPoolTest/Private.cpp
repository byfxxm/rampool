#include "stdafx.h"
#include "../RamPool/RamPool.h"

void* operator new(size_t nSize_)
{
	return RamPool_Malloc(nSize_);
}

void operator delete(void* p_)
{
	return RamPool_Free(p_);
}

void* operator new[](size_t nSize_)
{
	return RamPool_Malloc(nSize_);
}

void operator delete[](void* p_)
{
	return RamPool_Free(p_);
}
