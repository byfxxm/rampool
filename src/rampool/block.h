#pragma once
#include "stack.h"
#include "slot.h"

struct block : public stack<block>::node
{
	block(size_t, void*);
	~block();
	void* alloc(size_t);
	bool is_full();

	char* mem = nullptr;
	size_t mem_size = 0;
	slot** slots = nullptr;
	size_t slot_num = 0;
	size_t cur_slot = 0;
};