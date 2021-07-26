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

void* RamPool_Malloc(size_t nSize_, void* pRamPool_)
{
	if (pRamPool_ == nullptr)
		pRamPool_ = CRamPoolImp::Instance();

	return ((CRamPoolImp*)pRamPool_)->Malloc(nSize_);
}

void RamPool_Free(void* p_, void* pRamPool_)
{
	if (pRamPool_ == nullptr)
		pRamPool_ = CRamPoolImp::Instance();

	return ((CRamPoolImp*)pRamPool_)->Free(p_);
}

void RamPool_Destroy(void* pRamPool_)
{
	if (pRamPool_ == nullptr)
		pRamPool_ = CRamPoolImp::Instance();

	return ((CRamPoolImp*)pRamPool_)->Destroy();
}
