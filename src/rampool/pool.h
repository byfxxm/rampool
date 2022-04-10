#pragma once
#include "stack.h"
#include "block.h"
#include "slot.h"
#include "spin.h"

namespace rampool
{
	class pool_c
	{
	private:
		using mutex_t = spin_c;
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
		stack<block_s> __block_stack;
		stack<slot_s> __free_stack;
		size_t __size{ 0 };
		size_t __count{ 0 };
		size_t __total{ 0 };
		const void* __owner{ nullptr };
	};
}