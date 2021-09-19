#pragma once

template<typename Ty_>
class CLinkedList
{
public:
	struct Node
	{
		Ty_* m_pPrev = nullptr;
		Ty_* m_pNext = nullptr;
	};

	CLinkedList() = default;
	~CLinkedList() = default;

	bool PushBack(Ty_* p_)
	{
		if (!p_ || p_->m_pNext || p_->m_pPrev || p_ == m_pHead)
			return false;

		m_nCount++;

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

		m_nCount--;

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

	inline bool IsEmpty()
	{
		return m_pHead == nullptr;
	}

	void Erase(Ty_* p_)
	{
		assert(p_);
		m_nCount--;

		if (p_->m_pPrev)
			p_->m_pPrev->m_pNext = p_->m_pNext;
		else
			m_pHead = p_->m_pNext;

		if (p_->m_pNext)
			p_->m_pNext->m_pPrev = p_->m_pPrev;
		else
			m_pTail = p_->m_pPrev;
	}

	size_t GetCount()
	{
		return m_nCount;
	}

private:
	Ty_* m_pHead = nullptr;
	Ty_* m_pTail = nullptr;
	size_t m_nCount = 0;
};