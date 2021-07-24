#include "stdafx.h"
#include "Block.h"
#include "Slot.h"

CBlock::CBlock(int nSize_): Node<CBlock*>(this)
{
	printf("Add block(%d)\n", nSize_);

	m_nCurSlot = 0;
	m_nSize = ROUND(nSize_);

	int _nSlotSize = sizeof(Slot) + m_nSize;
	int _nMemSize = _nSlotSize * SLOTNUM;
	m_pMem = new char[_nMemSize];
	memset(m_pMem, 0, _nMemSize);

	int _nIndexOfMem = 0;
	for (int _i = 0; _i < SLOTNUM; _i++)
	{
		m_pSlots[_i] = new (&m_pMem[_nIndexOfMem]) Slot();
		m_pSlots[_i]->nSize = m_nSize;
		_nIndexOfMem += _nSlotSize;
	}
}

CBlock::~CBlock()
{
	delete[] m_pMem;
}

void* CBlock::Alloc()
{
	if (IsFull())
		return nullptr;
			
	return m_pSlots[m_nCurSlot++]->Mem;
}

bool CBlock::IsFull()
{
	return m_nCurSlot == SLOTNUM;
}