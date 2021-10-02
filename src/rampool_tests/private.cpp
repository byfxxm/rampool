#include "stdafx.h"
#include "../rampool/rampool.h"

void* operator new(size_t nSize_)
{
	return rp_malloc(nSize_);
}

void operator delete(void* p_)
{
	return rp_free(p_);
}

void* operator new[](size_t nSize_)
{
	return rp_malloc(nSize_);
}

void operator delete[](void* p_)
{
	return rp_free(p_);
}