#include "stdafx.h"
#include "Block.h"
#include "Slot.h"

CBlock::CBlock(int nSize_) : m_nCurSlot(0), m_nSlotNum(0)
{
	if (nSize_ <= MAXSIZE / 8)
		m_nSlotNum = 256;
	else if (nSize_ <= MAXSIZE)
		m_nSlotNum = 32;
	else
		assert(false);

	auto _nSize = ROUND(nSize_);
	auto _nSlotSize = sizeof(CSlot) + _nSize;
	auto _nMemSize = _nSlotSize * m_nSlotNum;
	m_pMem = (char*)VirtualAlloc(nullptr, _nMemSize, MEM_COMMIT, PAGE_READWRITE);
	m_pMem && memset((void*)m_pMem, 0, _nMemSize);

	m_ppSlots = new CSlot*[m_nSlotNum];
	auto _nIndexOfMem = 0;
	for (int _i = 0; _i < m_nSlotNum; _i++)
	{
		m_ppSlots[_i] = new(&m_pMem[_nIndexOfMem]) CSlot();
		m_ppSlots[_i]->m_nSize = _nSize;
		_nIndexOfMem += _nSlotSize;
	}
}

CBlock::~CBlock()
{
	delete[] m_ppSlots;
	VirtualFree(m_pMem, 0, MEM_RELEASE);
}

void* CBlock::Alloc(size_t nSize_)
{
	assert(!IsFull());
	assert(m_ppSlots[m_nCurSlot]->m_nValid == valid_t::SLOT_UNUSE);
	m_ppSlots[m_nCurSlot]->m_nActualSize = nSize_;
	m_ppSlots[m_nCurSlot]->m_nValid = valid_t::SLOT_USED;
	return m_ppSlots[m_nCurSlot++]->m_Mem;
}

bool CBlock::IsFull()
{
	return m_nCurSlot == m_nSlotNum;
}