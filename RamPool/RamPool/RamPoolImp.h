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
	void Clear();
	static CRamPoolImp* Instance();

private:
	CLinkedList<CBlock*> m_BlockLists[BLOCKNUM];
	CLinkedList<CSlot*> m_FreeLists[BLOCKNUM];
};