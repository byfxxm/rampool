#pragma once
#include "RamPool.h"
#include "Pool.h"

class CRamPoolImp
{
public:
	CRamPoolImp();
	~CRamPoolImp();
	static CRamPoolImp* Instance();
	void* Malloc(size_t);
	void Free(void*);
	void* Realloc(void*, size_t);
	void Destroy();
	void Leak(LeakInfo*);
	size_t Size(void*);
	void GC();

private:
	CPool m_Pools[POOLNUM];
};