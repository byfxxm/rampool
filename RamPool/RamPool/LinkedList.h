#pragma once
#include <functional>

template<class T>
struct Node
{
	Node(T&& data_) :Data(data_),pPrevious(nullptr), pNext(nullptr) {}
	T Data;
	Node* pPrevious;
	Node* pNext;
};

template<class T>
class CLinkedList
{
	using NODE = Node<T>;

public:

	CLinkedList()
	{
		m_pHead = nullptr;
		m_pTail = nullptr;
		m_pCur = m_pHead;
	}

	~CLinkedList()
	{
	}

	void PushBack(NODE* p_)
	{
		_ASSERT(p_ != nullptr);

		if (IsEmpty())
		{
			_ASSERT(m_pHead == nullptr && m_pTail == nullptr);
			m_pHead = m_pTail = p_;
			m_pHead->pPrevious = m_pHead->pNext = nullptr;
			return;
		}

		m_pTail->pNext = p_;
		p_->pPrevious = m_pTail;
		p_->pNext = nullptr;
		m_pTail = p_;
	}

	void PushFront(NODE* p_)
	{
		_ASSERT(p_ != nullptr);

		if (IsEmpty())
		{
			_ASSERT(m_pHead == nullptr && m_pTail == nullptr);
			m_pHead = m_pTail = p_;
			m_pHead->pPrevious = m_pHead->pNext = nullptr;
			return;
		}

		m_pHead->pPrevious = p_;
		p_->pNext = m_pHead;
		p_->pPrevious = nullptr;
		m_pHead = p_;
	}

	NODE* PopBack()
	{
		if (IsEmpty())
			return nullptr;

		auto _pOrgTail = m_pTail;
		m_pTail = m_pTail->pPrevious;
		m_pTail->pNext = nullptr;
		return _pOrgTail;
	}

	NODE* PopFront()
	{
		if (IsEmpty())
			return nullptr;

		auto _pOrgHead = m_pHead;
		m_pHead = m_pHead->pNext;
		m_pHead->pPrevious = nullptr;
		return _pOrgHead;
	}

	bool IsEmpty()
	{
		return m_pHead == nullptr;
	}

	NODE* Find(std::function<bool(NODE*)> func_) const
	{
		if (m_pHead == nullptr)
			return nullptr;

		NODE* _p = m_pHead;
		while (!func_(_p) && (_p = _p->pNext) != nullptr);

		return _p;
	}

private:
	NODE* m_pHead;
	NODE* m_pTail;
	NODE* m_pCur;
};