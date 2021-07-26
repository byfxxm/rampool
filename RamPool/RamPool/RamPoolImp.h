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
	CLinkedList m_BlockLists[BLOCKNUM];
	CLinkedList m_FreeLists[BLOCKNUM];
};