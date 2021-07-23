#pragma once


class CRamPool
{
public:
	CRamPool();
	~CRamPool();
	void* Malloc(int);
	void Free(void*);

private:
	
};