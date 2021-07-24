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

void* RamPool_Malloc(void* pRamPool_, unsigned nSize_)
{
	return ((CRamPoolImp*)pRamPool_)->Malloc(nSize_);
}

void RamPool_Free(void* pRamPool_, void* p_)
{
	((CRamPoolImp*)pRamPool_)->Free(p_);
}

void* rp_malloc(unsigned nSize_)
{
	return RamPool_Malloc(g_pRamPool, nSize_);
}

void rp_free(void* p_)
{
	RamPool_Free(g_pRamPool, p_);
}