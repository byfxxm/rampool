#pragma once
#include "stack.h"
#include "slot.h"

namespace rampool
{
	struct block_s : public stack<block_s>::node_s
	{
		block_s(size_t, const void*);
		~block_s();
		void* alloc(size_t);
		bool is_full();

		char* mem{ nullptr };
		size_t mem_size{ 0 };
		slot** slots{ nullptr };
		size_t slot_num{ 0 };
		size_t cur_slot{ 0 };
	};
}