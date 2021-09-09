#pragma once
#pragma warning(disable:4200)
#include "Node.h"

class CSlot: public CNode
{
public:
	CSlot() {}
	virtual ~CSlot() {}
	size_t m_nSize;
	char m_Mem[0];
};