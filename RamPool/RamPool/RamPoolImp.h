#pragma once
#include "LinkedList.h"

class CBlock;
class CSlot;
class CRamPoolImp
{
public:
	CRamPoolImp();
	~CRamPoolImp();
	void* Malloc(size_t);
	void Free(void*);
	void Destroy();
	static CRamPoolImp* Instance();

private:
	CLinkedList m_BlockLists[POOLNUM];
	CLinkedList m_FreeLists[POOLNUM];
};
