#include "stdafx.h"
#include "RamPoolImp.h"
#include "Block.h"
#include "Slot.h"

CRamPoolImp::CRamPoolImp()
{
}

CRamPoolImp::~CRamPoolImp()
{
	Clear();
}

CRamPoolImp* CRamPoolImp::Instance()
{
	static CRamPoolImp s_Inst;
	return &s_Inst;
}

void CRamPoolImp::Clear()
{
	for (auto& _list : m_BlockLists)
	{
		Node<CBlock*>* _p = nullptr;
		while ((_p = _list.PopFront()) != nullptr)
			delete _p;

		new(&_list) CLinkedList<CBlock*>();
	}

	for (auto& _list : m_FreeLists)
	{
		new(&_list) CLinkedList<CSlot*>();
	}
}

void* CRamPoolImp::Malloc(size_t nSize_)
{
	if (nSize_ == 0)
		return nullptr;

	int _index = BLOCKINDEX(nSize_);

	auto _p = m_FreeLists[_index].PopFront();
	if (_p != nullptr)
		return _p->m_Data->m_Mem;

	auto _pBlock = m_BlockLists[_index].Find([](Node<CBlock*>* p_)->bool
	{
		_ASSERT(p_ != nullptr);
		return !p_->m_Data->IsFull();
	});

	if (_pBlock != nullptr)
		return _pBlock->m_Data->Alloc();

	_pBlock = new CBlock(nSize_);
	m_BlockLists[_index].PushBack(_pBlock);
	return _pBlock->m_Data->Alloc();
}

void CRamPoolImp::Free(void* p_)
{
	if (p_ == nullptr)
		return;

	CSlot* _pSlot = POINTER_TO_SLOT(p_);
	int _index = BLOCKINDEX(_pSlot->m_nSize);
	m_FreeLists[_index].PushBack(_pSlot);
}