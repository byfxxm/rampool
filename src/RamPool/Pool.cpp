#include "stdafx.h"
#include "Pool.h"
#include "Block.h"
#include "Slot.h"

CPool::CPool()
{
	m_nSize = 0;
	m_nCount = 0;
}

CPool::~CPool()
{
}

void CPool::SetSize(size_t nSize_)
{
	m_nSize = nSize_;
}

size_t CPool::GetSize()
{
	return m_nSize;
}

void* CPool::Malloc()
{
	m_nCount++;

	auto _p = dynamic_cast<CSlot*>(m_FreeList.PopFront());
	if (_p != nullptr)
		return _p->m_Mem;

	auto _pBlock = dynamic_cast<CBlock*>(m_BlockList.Back());
	if (_pBlock == nullptr || _pBlock->IsFull())
	{
		_pBlock = new CBlock(m_nSize);
		m_BlockList.PushBack(_pBlock);
	}

	return _pBlock->Alloc();
}

void CPool::Free(void* p_)
{
	auto _pSlot = POINTER_TO_SLOT(p_);
	if (m_FreeList.PushBack(_pSlot))
		m_nCount--;
}

void CPool::Destroy()
{
	CNode* _p = nullptr;
	while ((_p = m_BlockList.PopFront()) != nullptr)
		delete _p;

	new(&m_BlockList) CLinkedList();
	new(&m_FreeList) CLinkedList();
	m_nCount = 0;
}

size_t CPool::GetCount()
{
	return m_nCount;
}