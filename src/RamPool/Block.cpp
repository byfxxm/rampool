#include "stdafx.h"
#include "Block.h"
#include "Slot.h"

Block::Block(size_t nSize_, void* pOwner_)
{
	if (nSize_ <= MAXSIZE / 8)
		m_nSlotNum = 32;
	else if (nSize_ <= MAXSIZE)
		m_nSlotNum = 4;
	else
		assert(false);

	auto _nSize = ROUND(nSize_);
	auto _nSlotSize = sizeof(Slot) + _nSize;
	m_nMemSize = _nSlotSize * m_nSlotNum;
	m_pMem = (char*)VirtualAlloc(nullptr, m_nMemSize, MEM_COMMIT, PAGE_READWRITE);
	m_pMem ? memset(m_pMem, 0, m_nMemSize) : throw bad_alloc();

	m_ppSlots = new Slot*[m_nSlotNum];
	auto _nIndexOfMem = 0;
	for (size_t _i = 0; _i < m_nSlotNum; _i++)
	{
		m_ppSlots[_i] = new(&m_pMem[_nIndexOfMem]) Slot();
		m_ppSlots[_i]->m_nSize = _nSize;
		m_ppSlots[_i]->m_pOwner = pOwner_;
		_nIndexOfMem += _nSlotSize;
	}
}

Block::~Block()
{
	delete[] m_ppSlots;
	VirtualFree(m_pMem, 0, MEM_RELEASE);
}

void* Block::Alloc(size_t nSize_)
{
	assert(!IsFull());
	assert(m_ppSlots[m_nCurSlot]->m_nValid == valid_t::SLOT_UNUSE);
	m_ppSlots[m_nCurSlot]->m_nValid = valid_t::SLOT_USED;
	m_ppSlots[m_nCurSlot]->m_nActualSize = nSize_;
	return m_ppSlots[m_nCurSlot++]->m_Mem;
}

bool Block::IsFull()
{
	return m_nCurSlot == m_nSlotNum;
}