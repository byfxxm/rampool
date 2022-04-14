#pragma once
#include "stack.h"
#include "block.h"
#include "slot.h"
#include "Spin.h"

namespace rampool {
	class pool {
	private:
		using mutex_t = Spin;
		using lock_t = std::lock_guard<mutex_t>;

	public:
		void initialize(size_t, const void*);
		size_t get_size();
		void* malloc(size_t);
		void free(void*);
		void destroy();
		size_t count();
		size_t total();
		void gc();
		bool need_gc();

	private:
		mutex_t __mutex;
		Stack<block> __block_Stack;
		Stack<slot> __free_Stack;
		size_t __size{ 0 };
		size_t count_{ 0 };
		size_t __total{ 0 };
		const void* __owner{ nullptr };
	};
}