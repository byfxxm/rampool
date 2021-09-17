#pragma once

template<typename Ty_>
class CLinkedList
{
public:
	struct Node
	{
		Node() : m_pPrev(nullptr), m_pNext(nullptr) {}
		Ty_* m_pPrev;
		Ty_* m_pNext;
	};

	CLinkedList() : m_pHead(nullptr), m_pTail(nullptr) {}
	~CLinkedList() = default;

	bool PushBack(Ty_* p_)
	{
		if (!p_ || p_->m_pNext || p_->m_pPrev || p_ == m_pHead)
			return false;

		if (IsEmpty())
		{
			assert(!m_pHead && !m_pTail);
			m_pHead = m_pTail = p_;
			return true;
		}

		m_pTail->m_pNext = p_;
		p_->m_pPrev = m_pTail;
		p_->m_pNext = nullptr;
		m_pTail = p_;
		return true;
	}

	Ty_* PopFront()
	{
		if (IsEmpty())
			return nullptr;

		auto _pHead = m_pHead;
		m_pHead = m_pHead->m_pNext;

		if (!m_pHead)
			m_pTail = nullptr;
		else
			m_pHead->m_pPrev = nullptr;

		_pHead->m_pPrev = _pHead->m_pNext = nullptr;
		return _pHead;
	}

	Ty_* Front()
	{
		return m_pHead;
	}

	Ty_* Back()
	{
		return m_pTail;
	}

	bool IsEmpty()
	{
		return m_pHead == nullptr;
	}

	void Erase(Ty_* p_)
	{
		assert(p_);

		if (p_->m_pPrev)
			p_->m_pPrev->m_pNext = p_->m_pNext;
		else
			m_pHead = p_->m_pNext;

		if (p_->m_pNext)
			p_->m_pNext->m_pPrev = p_->m_pPrev;
		else
			m_pTail = p_->m_pPrev;
	}

private:
	Ty_* m_pHead;
	Ty_* m_pTail;
};