#include "pch.h"
#include "Block.h"

CBlock::CBlock(int nSize_): Node<CBlock*>(*this)
{
	m_nSlotSize = ROUND(nSize_);

	m_nMemSize = m_nSlotSize * SLOTNUM;
	m_pMem = (char*)malloc(m_nMemSize);
	memset(m_pMem, 0, m_nMemSize);

	_ASSERT(SLOTNUM % 8 == 0);
	m_nMaskSize = SLOTNUM / 8;
	m_pMask = (char*)malloc(m_nMaskSize);
	memset(m_pMask, 0, m_nMaskSize);
}

CBlock::~CBlock()
{
	free(m_pMask);
	free(m_pMem);
}

void* CBlock::Alloc()
{
	char* _p = m_pMask;

	for (int _i = 0; _i < m_nMaskSize; _i++)
	{
		char _mask = 0b00000001;
		for (int _j = 0; _j < 8; _j++)
		{
			if ((m_pMask[_i] & _mask) == 0)
			{
				m_pMask[_i] |= _mask;
				return (void*)m_pMem[_i * 8 + _j];
			}

			_mask <<= 1;
		}
	}

	return nullptr;
}