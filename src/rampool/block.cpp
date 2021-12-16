#include "stdafx.h"
#include "block.h"
#include "slot.h"

block_s::block_s(size_t size, const void* owner)
{
	if (size <= MAXSIZE / 8)
		slot_num = 32;
	else if (size <= MAXSIZE)
		slot_num = 4;
	else
		assert(false);

	auto round_size = ROUND(size);
	auto slot_s_size = sizeof(slot_s) + round_size;
	mem_size = slot_s_size * slot_num;
	mem = (char*)VirtualAlloc(nullptr, mem_size, MEM_COMMIT, PAGE_READWRITE);
	mem ? memset(mem, 0, mem_size) : throw std::bad_alloc();

	slots = new slot_s * [slot_num];
	size_t index_of_mem = 0;
	for (size_t i = 0; i < slot_num; ++i)
	{
		slots[i] = new(&mem[index_of_mem]) slot_s();
		slots[i]->normalize_size = round_size;
		slots[i]->owner = owner;
		index_of_mem += slot_s_size;
	}
}

block_s::~block_s()
{
	delete[] slots;
	VirtualFree(mem, 0, MEM_RELEASE);
}

void* block_s::alloc(size_t size)
{
	assert(!is_full());
	assert(slots[cur_slot]->valid == slot_valid_t::UNUSE);
	slots[cur_slot]->valid = slot_valid_t::USED;
	slots[cur_slot]->actual_size = size;
	return slots[cur_slot++]->mem;
}

bool block_s::is_full()
{
	return cur_slot == slot_num;
}