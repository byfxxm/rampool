#pragma once
#include "stack.h"
#include "slot.h"

namespace rampool {
	struct Block : public Stack<Block>::Node {
		Block(size_t, const void*);
		~Block();
		void* Alloc(size_t);
		bool IsFull();

		char* mem{ nullptr };
		size_t mem_size{ 0 };
		Slot** slots{ nullptr };
		size_t slot_num{ 0 };
		size_t cur_slot{ 0 };
	};
}