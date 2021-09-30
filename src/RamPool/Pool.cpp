#include "stdafx.h"
#include "Pool.h"
#include "Block.h"
#include "Slot.h"

void CPool::SetSize(size_t nSize_)
{
	m_nSize = nSize_;
}

size_t CPool::GetSize()
{
	return m_nSize;
}

void* CPool::Malloc(size_t nSize_)
{
	unique_lock<mutex> _lock(m_Mutex);
	m_nCount++;
	m_nTotal += nSize_;

	auto _pSlot = m_FreeList.PopFront();
	if (_pSlot)
	{
		assert(_pSlot->m_nValid == valid_t::SLOT_DELETED);
		_pSlot->m_nValid = valid_t::SLOT_USED;
		_pSlot->m_nActualSize = nSize_;
		return _pSlot->m_Mem;
	}

	auto _pBlock = m_BlockList.Back();
	if (!_pBlock || _pBlock->IsFull())
	{
		_pBlock = new Block(m_nSize, this);
		m_BlockList.PushBack(_pBlock);
	}

	return _pBlock->Alloc(nSize_);
}

void CPool::Free(void* p_)
{
	unique_lock<mutex> _lock(m_Mutex);

	auto _pSlot = POINTER_TO_SLOT(p_);
	assert(_pSlot->m_pOwner == this);
	assert(_pSlot->m_nValid == valid_t::SLOT_USED);
	_pSlot->m_nValid = valid_t::SLOT_DELETED;

	if (!m_FreeList.PushBack(_pSlot))
		assert(false);

	m_nCount--;
	m_nTotal -= _pSlot->m_nActualSize;
	assert((int)m_nCount >= 0);
}

void CPool::Destroy()
{
	unique_lock<mutex> _lock(m_Mutex);

	Block* _p = nullptr;
	while ((_p = m_BlockList.PopFront()))
		delete _p;

	new(&m_BlockList) CLinkedList<Block>();
	new(&m_FreeList) CLinkedList<Slot>();
	m_nCount = 0;
	m_nTotal = 0;
}

size_t CPool::GetCount()
{
	return m_nCount;
}

atomic<size_t>& CPool::Total()
{
	return m_nTotal;
}

void CPool::GC()
{
	unique_lock<mutex> _lock(m_Mutex);

	Block* _pNext = NULL;
	for (auto _pBlock = m_BlockList.Front(); _pBlock; _pBlock = _pNext)
	{
		_pNext = _pBlock->m_pNext;

		size_t _index = 0;
		for (; _index < _pBlock->m_nCurSlot; _index++)
		{
			assert(_pBlock->m_ppSlots[_index]->m_nValid != valid_t::SLOT_UNUSE);
			if (_pBlock->m_ppSlots[_index]->m_nValid == valid_t::SLOT_USED)
				break;
		}

		if (_index == _pBlock->m_nCurSlot)
		{
			for (size_t _i = 0; _i < _pBlock->m_nCurSlot; _i++)
				_pBlock->m_ppSlots[_i]->m_nValid = valid_t::SLOT_UNUSE;

			for (auto _pSlot = m_FreeList.Front(); _pSlot; _pSlot = _pSlot->m_pNext)
			{
				if (_pSlot->m_nValid == valid_t::SLOT_UNUSE)
					m_FreeList.Erase(_pSlot);
			}

			m_BlockList.Erase(_pBlock);
			delete _pBlock;
		}
	}
}

bool CPool::NeedGC()
{
	return m_FreeList.GetCount() >= AUTOGC_THRESHOLD;
}