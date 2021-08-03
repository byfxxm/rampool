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

bool CLinkedList::PushBack(CNode* p_)
{
	if (p_ == nullptr || p_->m_pNext != nullptr || p_->m_pPrev != nullptr || p_ == m_pHead)
		return false;

	if (IsEmpty())
	{
		_ASSERT(m_pHead == nullptr && m_pTail == nullptr);
		m_pHead = m_pTail = p_;
		return true;
	}

	m_pTail->m_pNext = p_;
	p_->m_pPrev = m_pTail;
	p_->m_pNext = nullptr;
	m_pTail = p_;
	return true;
}

CNode* CLinkedList::PopFront()
{
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

CNode* CLinkedList::Front()
{
	return m_pHead;
}

CNode* CLinkedList::Back()
{
	return m_pTail;
}

inline bool CLinkedList::IsEmpty()
{
	return m_pHead == nullptr;
}