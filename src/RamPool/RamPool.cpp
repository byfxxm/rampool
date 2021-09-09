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

void* RamPool_Realloc(void* pRamPool_, void* p_, size_t nSize_)
{
	return ((CRamPoolImp*)pRamPool_)->Realloc(p_, nSize_);
}

void RamPool_Destroy(void* pRamPool_)
{
	return ((CRamPoolImp*)pRamPool_)->Destroy();
}

void RamPool_Leak(void* pRamPool_, size_t* pCount_, size_t* pTotalSize_)
{
	return ((CRamPoolImp*)pRamPool_)->Leak(pCount_, pTotalSize_);
}

size_t RamPool_Size(void* pRamPool_, void* p_)
{
	return ((CRamPoolImp*)pRamPool_)->Size(p_);
}

void* rp_malloc(size_t nSize_)
{
	return CRamPoolImp::Instance()->Malloc(nSize_);
}

void rp_free(void* p_)
{
	return CRamPoolImp::Instance()->Free(p_);
}

void* rp_realloc(void* p_, size_t nSize_)
{
	return CRamPoolImp::Instance()->Realloc(p_, nSize_);
}

void rp_destroy()
{
	return CRamPoolImp::Instance()->Destroy();
}

void rp_leak(size_t* pCount_, size_t* pTotalSize_)
{
	return CRamPoolImp::Instance()->Leak(pCount_, pTotalSize_);
}

size_t rp_size(void* p_)
{
	return CRamPoolImp::Instance()->Size(p_);
}