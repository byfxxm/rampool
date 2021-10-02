#pragma once

template<typename ty>
class stack
{
public:
	struct node
	{
		ty* m_pPrev = nullptr;
		ty* m_pNext = nullptr;
	};

	stack() = default;
	~stack() = default;

	void Push(ty* p)
	{
		assert(p);
		_count++;
		p->m_pNext = m_pTop;
		m_pTop && (m_pTop->m_pPrev = p, 0);
		m_pTop = p;
	}

	void Pop()
	{
		assert(m_pTop);
		_count--;
		m_pTop = m_pTop->m_pNext;
		m_pTop && (m_pTop->m_pPrev = nullptr, 0);
	}

	ty* Top()
	{
		return m_pTop;
	}

	void Erase(ty* p)
	{
		assert(p);
		_count--;

		if (p->m_pPrev)
			p->m_pPrev->m_pNext = p->m_pNext;
		else
			m_pTop = p->m_pNext;

		if (p->m_pNext)
			p->m_pNext->m_pPrev = p->m_pPrev;
	}

	size_t Count()
	{
		return _count;
	}

private:
	ty* m_pTop = nullptr;
	size_t _count = 0;
};