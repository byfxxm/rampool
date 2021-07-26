#include "stdafx.h"
#include "Block.h"
#include "Slot.h"

CBlock::CBlock(int nSize_)
{
	m_nCurSlot = 0;

	auto _nSize = ROUND(nSize_);
	auto _nSlotSize = sizeof(CSlot) + _nSize;
	auto _nMemSize = _nSlotSize * SLOTNUM;
	m_pMem = (char*)VirtualAlloc(nullptr, _nMemSize, MEM_COMMIT, PAGE_READWRITE);
	memset(m_pMem, 0, _nMemSize);

	auto _nIndexOfMem = 0;
	for (int _i = 0; _i < SLOTNUM; _i++)
	{
		m_pSlots[_i] = new (&m_pMem[_nIndexOfMem]) CSlot();
		m_pSlots[_i]->m_nSize = _nSize;
		_nIndexOfMem += _nSlotSize;
	}
}

CBlock::~CBlock()
{
	VirtualFree(m_pMem, 0, MEM_RELEASE);
}

void* CBlock::Alloc()
{
	if (IsFull())
		return nullptr;
			
	return m_pSlots[m_nCurSlot++]->m_Mem;
}

bool CBlock::IsFull()
{
	return m_nCurSlot == SLOTNUM;
}
