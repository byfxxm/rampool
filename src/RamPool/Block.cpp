#include "stdafx.h"
#include "Block.h"
#include "Slot.h"

CBlock::CBlock(size_t nSize_) : m_nCurSlot(0), m_nSlotNum(0)
{
	if (nSize_ <= MAXSIZE / 8)
		m_nSlotNum = 64;
	else if (nSize_ <= MAXSIZE)
		m_nSlotNum = 8;
	else
		assert(false);

	auto _nSize = ROUND(nSize_);
	auto _nSlotSize = sizeof(CSlot) + _nSize;
	m_nMemSize = _nSlotSize * m_nSlotNum;
	m_pMem = (char*)VirtualAlloc(nullptr, m_nMemSize, MEM_COMMIT, PAGE_READWRITE);
	m_pMem && memset(m_pMem, 0, m_nMemSize);

	m_ppSlots = new CSlot*[m_nSlotNum];
	auto _nIndexOfMem = 0;
	for (size_t _i = 0; _i < m_nSlotNum; _i++)
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