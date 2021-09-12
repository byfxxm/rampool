#pragma once
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
	void Leak(size_t*, size_t*);
	size_t Size(void*);
	static CRamPoolImp* Instance();

private:
	CPool m_Pools[POOLNUM];
};