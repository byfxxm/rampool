#pragma once
#include "Pool.h"

class CBlock;
class CSlot;
class CRamPoolImp
{
public:
	CRamPoolImp();
	~CRamPoolImp();
	void* Malloc(size_t);
	void Free(void*);
	void* Realloc(void*, size_t);
	void Destroy();
	size_t Leak();
	size_t Size(void*);
	static CRamPoolImp* Instance();

private:
	CPool m_Pools[POOLNUM];
};