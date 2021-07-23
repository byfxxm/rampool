#include "stdafx.h"
#include "RamPoolImp.h"
#include "Block.h"
#include "Slot.h"

CRamPoolImp::CRamPoolImp()
{
}

CRamPoolImp::~CRamPoolImp()
{
	for (auto _block : m_Blocks)
	{
		_block.Find([](Node<CBlock*>* p_)->bool
		{
			_ASSERT(p_ != nullptr);
			delete p_;

			return false;
		});
	}
}

void* CRamPoolImp::Malloc(int nSize_)
{
	int _index = BLOCKINDEX(nSize_);

	if (!m_FreeList[_index].IsEmpty())
	{
		return m_FreeList[_index].PopFront()->Data;
	}

	auto _pBlock = m_Blocks[_index].Find([](Node<CBlock*>* p_)->bool
	{
		_ASSERT(p_ != nullptr);
		
		if (p_->Data->Alloc() == nullptr)
			return false;

		return true;
	});

	if (_pBlock != nullptr)
		return _pBlock;

	_pBlock = new CBlock(nSize_);
	m_Blocks[_index].PushBack(_pBlock);
	return _pBlock->Data->Alloc();
}

void CRamPoolImp::Free(void* p_)
{
	CSlot* _pSlot = POINTER_TO_SLOT(p_);
	m_FreeList[BLOCKINDEX(_pSlot->nSize)].PushBack(_pSlot);
}