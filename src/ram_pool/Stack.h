#pragma once

template<typename ty>
class stack
{
public:
	struct node
	{
		ty* prev = nullptr;
		ty* next = nullptr;
	};

	stack() = default;
	~stack() = default;

	void push(ty* p)
	{
		assert(p);
		_count++;
		p->next = _top;
		_top && (_top->prev = p, 0);
		_top = p;
	}

	void pop()
	{
		assert(_top);
		_count--;
		_top = _top->next;
		_top && (_top->prev = nullptr, 0);
	}

	ty* top()
	{
		return _top;
	}

	void erase(ty* p)
	{
		assert(p);
		_count--;

		if (p->prev)
			p->prev->next = p->next;
		else
			_top = p->next;

		if (p->next)
			p->next->prev = p->prev;
	}

	size_t count()
	{
		return _count;
	}

private:
	ty* _top = nullptr;
	size_t _count = 0;
};