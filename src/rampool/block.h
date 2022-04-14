#pragma once
#include "stack.h"
#include "Slot.h"

namespace rampool {
	struct block : public Stack<block>::Node {
		block(size_t, const void*);
		~block();
		void* alloc(size_t);
		bool is_full();

		char* mem{ nullptr };
		size_t mem_size{ 0 };
		Slot** slots{ nullptr };
		size_t slot_num{ 0 };
		size_t cur_slot{ 0 };
	};
}