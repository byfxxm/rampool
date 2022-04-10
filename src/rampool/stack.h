#pragma once

namespace rampool
{
	template<typename T>
	class stack
	{
	public:
		struct node_s
		{
			T* prev = nullptr;
			T* next = nullptr;
		};

		void push(T* p)
		{
			assert(p);
			++__count;
			p->next = __top;
			__top && (__top->prev = p, 0);
			__top = p;
		}

		T* pop()
		{
			if (!__top)
				return nullptr;

			auto ret = __top;
			--__count;
			__top = __top->next;
			__top && (__top->prev = nullptr, 0);
			assert(__count >= 0);
			return ret;
		}

		T* top() const
		{
			return __top;
		}

		void erase(T* p)
		{
			assert(p);
			--__count;

			if (p->prev)
				p->prev->next = p->next;
			else
				__top = p->next;

			if (p->next)
				p->next->prev = p->prev;
		}

		size_t count() const
		{
			return __count;
		}

	private:
		T* __top{ nullptr };
		size_t __count{ 0 };
	};
}