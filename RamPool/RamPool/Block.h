#pragma once
#include "LinkedList.h"

class CBlock: public Node<CBlock*>
{
public:
	CBlock(int);
	virtual ~CBlock();
	void* Alloc();

private:
	char* m_pMem;
	int m_nMemSize;
	char* m_pMask;
	int m_nMaskSize;
	int m_nSlotSize;
};