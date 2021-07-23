#pragma once
#include "LinkedList.h"

class CBlock;
class CRamPoolImp
{
	using NODE = Node<CBlock*>;
public:
	CRamPoolImp();
	~CRamPoolImp();
	void* Malloc(int);
	void Free(void*);

private:
	CLinkedList<CBlock*> m_Blocks[BLOCKNUM];
	//LINKEDLIST m_BlockList;
	CLinkedList<CBlock*> m_FreeList[BLOCKNUM];
};