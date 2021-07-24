#pragma once
#include "LinkedList.h"

class CBlock;
struct Slot;
class CRamPoolImp
{
public:
	CRamPoolImp();
	~CRamPoolImp();
	void* Malloc(int);
	void Free(void*);

private:
	CLinkedList<CBlock*> m_Blocks[BLOCKNUM];
	//LINKEDLIST m_BlockList;
	CLinkedList<Slot*> m_FreeList[BLOCKNUM];
};