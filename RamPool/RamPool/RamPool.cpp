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