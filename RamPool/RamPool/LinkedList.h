#pragma once
#include <functional>

template<class T>
struct Node
{
	Node(T data_) :Data(data_), pPrevious(nullptr), pNext(nullptr) {}
	T Data;
	Node* pPrevious;
	Node* pNext;
};

template<class T>
class CLinkedList
{
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

	void PushBack(Node<T>* p_)
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

	void PushFront(Node<T>* p_)
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

	Node<T>* PopBack()
	{
		if (IsEmpty())
			return nullptr;

		auto _pOrgTail = m_pTail;
		m_pTail = m_pTail->pPrevious;
		m_pTail->pNext = nullptr;
		return _pOrgTail;
	}

	Node<T>* PopFront()
	{
		if (IsEmpty())
			return nullptr;

		auto _pOrgHead = m_pHead;
		m_pHead = m_pHead->pNext;
		if (m_pHead == nullptr)
			m_pTail = nullptr;
		else
			m_pHead->pPrevious = nullptr;
		return _pOrgHead;
	}

	bool IsEmpty()
	{
		return m_pHead == nullptr;
	}

	Node<T>* Find(std::function<bool(Node<T>*)> func_) const
	{
		if (m_pHead == nullptr)
			return nullptr;

		Node<T>* _p = m_pHead;
		while (!func_(_p) && (_p = _p->pNext) != nullptr);

		return _p;
	}

private:
	Node<T>* m_pHead;
	Node<T>* m_pTail;
	Node<T>* m_pCur;
};
