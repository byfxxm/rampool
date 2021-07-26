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
	unique_lock<mutex> _locker(m_Mutex);
	_ASSERT(p_ != nullptr);

	if (IsEmpty())
	{
		_ASSERT(m_pHead == nullptr && m_pTail == nullptr);
		m_pHead = m_pTail = p_;
		m_pHead->m_pPrevious = m_pHead->m_pNext = nullptr;
		return;
	}

	m_pTail->m_pNext = p_;
	p_->m_pPrevious = m_pTail;
	p_->m_pNext = nullptr;
	m_pTail = p_;
}

CNode* CLinkedList::PopFront()
{
	unique_lock<mutex> _locker(m_Mutex);

	if (IsEmpty())
		return nullptr;

	auto _pHead = m_pHead;
	m_pHead = m_pHead->m_pNext;

	if (m_pHead == nullptr)
		m_pTail = nullptr;
	else
		m_pHead->m_pPrevious = nullptr;

	return _pHead;
}

CNode* CLinkedList::Find(std::function<bool(CNode*)> func_)
{
	unique_lock<mutex> _locker(m_Mutex);

	auto _p = m_pHead;
	while (_p != nullptr)
	{
		if (func_(_p))
			break;

		_p = _p->m_pNext;
	}

	return _p;
}

bool CLinkedList::IsEmpty()
{
	return m_pHead == nullptr;
}