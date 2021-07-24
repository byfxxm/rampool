#pragma once
#include "LinkedList.h"

class CBlock;
class CSlot;
class CRamPoolImp
{
public:
	CRamPoolImp();
	~CRamPoolImp();
	void* Malloc(int);
	void Free(void*);

private:
	CLinkedList<CBlock*> m_BlockList[BLOCKNUM];
	CLinkedList<CSlot*> m_FreeList[BLOCKNUM];
};