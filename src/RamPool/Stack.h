#pragma once

template<typename Ty_>
class CStack
{
public:
	struct Node
	{
		Ty_* m_pPrev = nullptr;
		Ty_* m_pNext = nullptr;
	};

	CStack() = default;
	~CStack() = default;

	void Push(Ty_* p_)
	{
		assert(p_);
		m_nCount++;
		p_->m_pNext = m_pTop;
		m_pTop && (m_pTop->m_pPrev = p_, 0);
		m_pTop = p_;
	}

	void Pop()
	{
		assert(m_pTop);
		m_nCount--;
		m_pTop = m_pTop->m_pNext;
		m_pTop && (m_pTop->m_pPrev = nullptr, 0);
	}

	Ty_* Top()
	{
		return m_pTop;
	}

	void Erase(Ty_* p_)
	{
		assert(p_);
		m_nCount--;

		if (p_->m_pPrev)
			p_->m_pPrev->m_pNext = p_->m_pNext;
		else
			m_pTop = p_->m_pNext;

		if (p_->m_pNext)
			p_->m_pNext->m_pPrev = p_->m_pPrev;
	}

	size_t Count()
	{
		return m_nCount;
	}

private:
	Ty_* m_pTop = nullptr;
	size_t m_nCount = 0;
};