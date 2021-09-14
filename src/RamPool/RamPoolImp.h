#pragma once
#include "RamPool.h"
#include "Pool.h"

class CRamPoolImp
{
public:
	CRamPoolImp();
	~CRamPoolImp();
	void* Malloc(size_t);
	void Free(void*);
	void* Realloc(void*, size_t);
	void Destroy();
	void Leak(LeakInfo*);
	size_t Size(void*);
	static CRamPoolImp* Instance();

private:
	CPool m_Pools[POOLNUM];
};