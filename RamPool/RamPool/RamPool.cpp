#include "pch.h"
#include "RamPool.h"
#include "RamPoolImp.h"


void* RamPool_Create()
{
	return new CRamPool();
}

void RamPool_Delete(void* pRamPool_)
{
	delete pRamPool_;
}

void* RamPool_Malloc(void* pRamPool_, unsigned nSize_)
{
	return ((CRamPool*)pRamPool_)->Malloc(nSize_);
}

void RamPool_Free(void* pRamPool_, void* p_)
{
	((CRamPool*)pRamPool_)->Free(p_);
}