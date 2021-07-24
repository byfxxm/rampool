#pragma once
#include "LinkedList.h"

struct Slot;
class CBlock: public Node<CBlock*>
{
public:
	CBlock(int);
	~CBlock();
	void* Alloc();
	bool IsFull();

private:
	char* m_pMem;
	int m_nSize;
	Slot* m_pSlots[SLOTNUM];
	int m_nCurSlot;
};