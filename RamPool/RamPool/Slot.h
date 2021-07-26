#pragma once
#pragma warning(disable:4200)
#include "Node.h"

class CSlot: public CNode
{
public:
	CSlot() {}
	virtual ~CSlot() {}
	int m_nSize;
	char m_Mem[0];
};

#define POINTER_TO_SLOT(p) ((CSlot*)((char*)(p) - sizeof(CSlot)))
