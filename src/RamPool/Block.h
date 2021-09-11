#pragma once
#include "Node.h"

class CSlot;
class CBlock : public CNode
{
public:
	CBlock(size_t, void*);
	virtual ~CBlock() override;
	void* Alloc(size_t);
	bool IsFull();

private:
	char* m_pMem;
	size_t m_nMemSize;
	CSlot** m_ppSlots;
	size_t m_nSlotNum;
	size_t m_nCurSlot;
};