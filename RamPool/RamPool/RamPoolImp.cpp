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
	if (nSize_ <= 0)
		return nullptr;

	int _index = BLOCKINDEX(nSize_);

	if (!m_FreeList[_index].IsEmpty())
	{
		return m_FreeList[_index].PopFront()->Data->Mem;
	}

	auto _pBlock = m_Blocks[_index].Find([](Node<CBlock*>* p_)->bool
	{
		_ASSERT(p_ != nullptr);
		return !p_->Data->IsFull();
	});

	if (_pBlock != nullptr)
		return _pBlock->Data->Alloc();

	_pBlock = new CBlock(nSize_);
	m_Blocks[_index].PushBack(_pBlock);
	return _pBlock->Data->Alloc();
}

void CRamPoolImp::Free(void* p_)
{
	if (p_ == nullptr)
		return;

	Slot* _pSlot = POINTER_TO_SLOT(p_);
	int _index = BLOCKINDEX(_pSlot->nSize);
	m_FreeList[_index].PushBack(_pSlot);
}