#include "stdafx.h"
#include "Block.h"
#include "Slot.h"

CBlock::CBlock(int nSize_): Node<CBlock*>(this)
{
	m_nSize = ROUND(nSize_);

	int _nSlotSize = sizeof(CSlot) + m_nSize;
	int _nMemSize = _nSlotSize * SLOTNUM;
	m_pMem = new char[_nMemSize];
	memset(m_pMem, 0, _nMemSize);

	int _nData = 0;
	for (int _i = 0; _i < SLOTNUM; _i++)
	{
		m_pSlots[_i] = new (&m_pMem[_nData]) CSlot();
		_nData += _nSlotSize;
	}
}

CBlock::~CBlock()
{
	delete[] m_pMem;
}

void* CBlock::Alloc()
{
	for (int _i = 0; _i < SLOTNUM; _i++)
	{
		if (!m_pSlots[_i]->bUsed)
		{
			m_pSlots[_i]->bUsed = true;
			return m_pSlots[_i]->Data;
		}
	}

	return nullptr;
}