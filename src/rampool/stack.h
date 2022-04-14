#pragma once

namespace rampool {
	template<typename T>
	class Stack {
	public:
		struct Node {
			T* prev = nullptr;
			T* next = nullptr;
		};

		void Push(T* p) {
			assert(p);
			++count_;
			p->next = top_;
			top_ && (top_->prev = p, 0);
			top_ = p;
		}

		T* Pop() {
			if (!top_)
				return nullptr;

			auto ret = top_;
			--count_;
			top_ = top_->next;
			top_ && (top_->prev = nullptr, 0);
			assert(count_ >= 0);
			return ret;
		}

		T* Top() const {
			return top_;
		}

		void Erase(T* p) {
			assert(p);
			--count_;

			if (p->prev)
				p->prev->next = p->next;
			else
				top_ = p->next;

			if (p->next)
				p->next->prev = p->prev;
		}

		size_t Count() const {
			return count_;
		}

	private:
		T* top_{ nullptr };
		size_t count_{ 0 };
	};
}