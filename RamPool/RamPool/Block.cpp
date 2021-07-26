#include "stdafx.h"
#include "Block.h"
#include "Slot.h"

CBlock::CBlock(int nSize_)
{
	m_nCurSlot = 0;
	m_nSlotNum = SLOTNUM;
	//if (nSize_ <= MAXSIZE / 16)
	//	m_nSlotNum = 1024;
	//else if (nSize_ <= MAXSIZE / 8)
	//	m_nSlotNum = 512;
	//else if (nSize_ <= MAXSIZE / 4)
	//	m_nSlotNum = 256;
	//else if (nSize_ <= MAXSIZE / 2)
	//	m_nSlotNum = 128;
	//else if (nSize_ <= MAXSIZE)
	//	m_nSlotNum = 64;

	auto _nSize = ROUND(nSize_);
	auto _nSlotSize = sizeof(CSlot) + _nSize;
	auto _nMemSize = _nSlotSize * m_nSlotNum;
	m_pMem = (char*)VirtualAlloc(nullptr, _nMemSize, MEM_COMMIT, PAGE_READWRITE);
	memset(m_pMem, 0, _nMemSize);

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

void* CBlock::Alloc()
{
	if (IsFull())
		return nullptr;
			
	return m_ppSlots[m_nCurSlot++]->m_Mem;
}

bool CBlock::IsFull()
{
	return m_nCurSlot == m_nSlotNum;
}
