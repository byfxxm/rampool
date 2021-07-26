#pragma once
#include "Node.h"

class CSlot;
class CBlock: public CNode
{
public:
	CBlock(int);
	virtual ~CBlock();
	void* Alloc();
	bool IsFull();

private:
	char* m_pMem;
	CSlot* m_pSlots[SLOTNUM];
	int m_nCurSlot;
};
