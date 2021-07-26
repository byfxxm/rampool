#include "stdafx.h"
#include "RamPool.h"
#include "RamPoolImp.h"


void* RamPool_Create()
{
	return new CRamPoolImp();
}

void RamPool_Delete(void* pRamPool_)
{
	delete (CRamPoolImp*)pRamPool_;
}

void* RamPool_Malloc(void* pRamPool_, size_t nSize_)
{
	return ((CRamPoolImp*)pRamPool_)->Malloc(nSize_);
}

void RamPool_Free(void* pRamPool_, void* p_)
{
	return ((CRamPoolImp*)pRamPool_)->Free(p_);
}

void RamPool_Clear(void* pRamPool_)
{
	return ((CRamPoolImp*)pRamPool_)->Clear();
}

void* rp_malloc(size_t nSize_)
{
	return CRamPoolImp::Instance()->Malloc(nSize_);
}

void rp_free(void* p_)
{
	return CRamPoolImp::Instance()->Free(p_);
}

void rp_clear()
{
	return CRamPoolImp::Instance()->Clear();
}