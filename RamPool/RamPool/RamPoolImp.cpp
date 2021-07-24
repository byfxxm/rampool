#include "stdafx.h"
#include "RamPoolImp.h"
#include "Block.h"
#include "Slot.h"

CRamPoolImp::CRamPoolImp()
{
}

CRamPoolImp::~CRamPoolImp()
{
	for (auto _block : m_BlockList)
	{
		while (!_block.IsEmpty())
		{
			delete _block.PopFront();
		}
	}
}

void* CRamPoolImp::Malloc(int nSize_)
{
	if (nSize_ <= 0)
		return nullptr;

	int _index = BLOCKINDEX(nSize_);

	if (!m_FreeList[_index].IsEmpty())
	{
		return m_FreeList[_index].PopFront()->m_Data->m_Mem;
	}

	auto _pBlock = m_BlockList[_index].Find([](Node<CBlock*>* p_)->bool
	{
		_ASSERT(p_ != nullptr);
		return !p_->m_Data->IsFull();
	});

	if (_pBlock != nullptr)
		return _pBlock->m_Data->Alloc();

	_pBlock = new CBlock(nSize_);
	m_BlockList[_index].PushBack(_pBlock);
	return _pBlock->m_Data->Alloc();
}

void CRamPoolImp::Free(void* p_)
{
	if (p_ == nullptr)
		return;

	CSlot* _pSlot = POINTER_TO_SLOT(p_);
	int _index = BLOCKINDEX(_pSlot->m_nSize);
	m_FreeList[_index].PushBack(_pSlot);
}