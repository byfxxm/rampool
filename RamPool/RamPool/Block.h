#pragma once
#include "LinkedList.h"

class CSlot;
class CBlock: public Node<CBlock*>
{
public:
	CBlock(int);
	virtual ~CBlock();
	void* Alloc();

private:
	char* m_pMem;
	int m_nSize;
	CSlot* m_pSlots[SLOTNUM];
};