#pragma once
#include <functional>

template<class T>
class Node
{
public:
	Node(T data_) :m_Data(data_), m_pPrevious(nullptr), m_pNext(nullptr) {}
	virtual ~Node() {}
	T m_Data;
	Node* m_pPrevious;
	Node* m_pNext;
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
			m_pHead->m_pPrevious = m_pHead->m_pNext = nullptr;
			return;
		}

		m_pTail->m_pNext = p_;
		p_->m_pPrevious = m_pTail;
		p_->m_pNext = nullptr;
		m_pTail = p_;
	}

	Node<T>* PopFront()
	{
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

	bool IsEmpty()
	{
		return m_pHead == nullptr;
	}

	Node<T>* Find(std::function<bool(Node<T>*)> func_) const
	{
		Node<T>* _p = m_pHead;

		while (_p != nullptr)
		{
			if (func_(_p))
				break;

			_p = _p->m_pNext;
		}

		return _p;
	}

private:
	Node<T>* m_pHead;
	Node<T>* m_pTail;
	Node<T>* m_pCur;
};
