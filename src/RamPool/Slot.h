#pragma once
#pragma warning(disable:4200)
#include "Node.h"

class CSlot: public CNode
{
public:
	CSlot() = default;
	virtual ~CSlot() = default;

	size_t m_nSize;
	size_t m_nActualSize;

	char m_Mem[0];
};