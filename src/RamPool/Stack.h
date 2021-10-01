#pragma once

template<typename Ty_>
class CStack
{
public:
	struct Node
	{
		Ty_* m_pNext = nullptr;
	};

	CStack() = default;
	~CStack() = default;

	void Push(Ty_* p_)
	{
		assert(p_);
		m_nCount++;
		p_->m_pNext = m_pTop;
		m_pTop = p_;
	}

	void Pop()
	{
		assert(m_pTop);
		m_nCount--;
		m_pTop && (m_pTop = m_pTop->m_pNext);
	}

	Ty_* Top()
	{
		return m_pTop;
	}

	void Erase(Ty_* p_)
	{
		assert(p_);
		m_nCount--;

		auto _p = m_pTop;
		Ty_* _pLast = nullptr;
		while (_p)
		{
			if (_p == p_)
			{
				_pLast ? (_pLast->m_pNext = _p->m_pNext) : (m_pTop = _p->m_pNext);
				break;
			}

			_pLast = _p;
			_p = _p->m_pNext;
		}
	}

	size_t Count()
	{
		return m_nCount;
	}

private:
	Ty_* m_pTop = nullptr;
	size_t m_nCount = 0;
};