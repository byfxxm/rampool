#include "stdafx.h"
#include "LinkedList.h"
#include "Node.h"

CLinkedList::CLinkedList()
{
	m_pHead = nullptr;
	m_pTail = nullptr;
}

CLinkedList::~CLinkedList()
{
}

void CLinkedList::PushBack(CNode* p_)
{
	unique_lock<mutex> _lock(m_Mutex);

	if (p_ == nullptr || p_->m_pNext != nullptr || p_->m_pPrev != nullptr || p_ == m_pHead)
		return;

	if (IsEmpty())
	{
		_ASSERT(m_pHead == nullptr && m_pTail == nullptr);
		m_pHead = m_pTail = p_;
		return;
	}

	m_pTail->m_pNext = p_;
	p_->m_pPrev = m_pTail;
	p_->m_pNext = nullptr;
	m_pTail = p_;
}

CNode* CLinkedList::PopFront()
{
	unique_lock<mutex> _lock(m_Mutex);

	if (IsEmpty())
		return nullptr;

	auto _pHead = m_pHead;
	m_pHead = m_pHead->m_pNext;

	if (m_pHead == nullptr)
		m_pTail = nullptr;
	else
		m_pHead->m_pPrev = nullptr;

	_pHead->m_pPrev = _pHead->m_pNext = nullptr;
	return _pHead;
}

CNode* CLinkedList::Find(std::function<bool(CNode*)> func_)
{
	unique_lock<mutex> _lock(m_Mutex);
	auto _p = m_pHead;

	while (_p != nullptr)
	{
		if (func_(_p))
			break;

		_p = _p->m_pNext;
	}

	return _p;
}

inline bool CLinkedList::IsEmpty()
{
	return m_pHead == nullptr;
}