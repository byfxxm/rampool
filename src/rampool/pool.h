#pragma once
#include "stack.h"
#include "block.h"
#include "Slot.h"
#include "Spin.h"

namespace rampool {
	class pool {
	private:
		using mutex_t = Spin;
		using lock_t = std::lock_guard<mutex_t>;

	public:
		void initialize(size_t, const void*);
		size_t get_size();
		void* Malloc(size_t);
		void Free(void*);
		void Destroy();
		size_t count();
		size_t total();
		void Gc();
		bool need_gc();

	private:
		mutex_t __mutex;
		Stack<block> __block_Stack;
		Stack<Slot> __free_Stack;
		size_t __size{ 0 };
		size_t count_{ 0 };
		size_t __total{ 0 };
		const void* __owner{ nullptr };
	};
}