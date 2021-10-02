#pragma once
#include "stack.h"
#include "slot.h"

struct block : public stack<block>::node
{
	block(size_t, void*);
	~block();
	void* alloc(size_t);
	bool is_full();

	char* _mem = nullptr;
	size_t _mem_size = 0;
	slot** _slots = nullptr;
	size_t _slot_num = 0;
	size_t _cur_slot = 0;
};