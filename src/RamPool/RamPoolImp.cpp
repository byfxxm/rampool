#include "stdafx.h"
#include "RamPoolImp.h"
#include "Block.h"
#include "Slot.h"

CRamPoolImp::CRamPoolImp()
{
}

CRamPoolImp::~CRamPoolImp()
{
	Destroy();
}

CRamPoolImp* CRamPoolImp::Instance()
{
	static CRamPoolImp s_Inst;
	return &s_Inst;
}

void CRamPoolImp::Destroy()
{
	for (auto& _list : m_BlockLists)
	{
		CNode* _p = nullptr;
		while ((_p = _list.PopFront()) != nullptr)
			delete _p;

		new(&_list) CLinkedList();
	}

	for (auto& _list : m_FreeLists)
	{
		new(&_list) CLinkedList();
	}
}

void* CRamPoolImp::Malloc(size_t nSize_)
{
	if (nSize_ == 0 || nSize_ > MAXSIZE)
		return nullptr;

	auto _index = POOLINDEX(nSize_);

	auto _p = dynamic_cast<CSlot*>(m_FreeLists[_index].PopFront());
	if (_p != nullptr)
		return _p->m_Mem;

	auto _pBlock = dynamic_cast<CBlock*>(m_BlockLists[_index].Find([](CNode* p_)->bool
	{
		auto _p = dynamic_cast<CBlock*>(p_);
		_ASSERT(_p != nullptr);
		return !_p->IsFull();
	}));

	if (_pBlock != nullptr)
		return _pBlock->Alloc();

	_pBlock = new CBlock(nSize_);
	m_BlockLists[_index].PushBack(_pBlock);
	return _pBlock->Alloc();
}

void CRamPoolImp::Free(void* p_)
{
	if (p_ == nullptr)
		return;

	auto _pSlot = POINTER_TO_SLOT(p_);
	m_FreeLists[POOLINDEX(_pSlot->m_nSize)].PushBack(_pSlot);
}