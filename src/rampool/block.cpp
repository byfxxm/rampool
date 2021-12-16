#include "stdafx.h"
#include "block.h"
#include "slot.h"

block_s::block_s(size_t size, const void* owner)
{
	if (size <= MAXSIZE / 8)
		slot_s_num = 32;
	else if (size <= MAXSIZE)
		slot_s_num = 4;
	else
		assert(false);

	auto round_size = ROUND(size);
	auto slot_s_size = sizeof(slot_s) + round_size;
	mem_size = slot_s_size * slot_s_num;
	mem = (char*)VirtualAlloc(nullptr, mem_size, MEM_COMMIT, PAGE_READWRITE);
	mem ? memset(mem, 0, mem_size) : throw std::bad_alloc();

	slot_ss = new slot_s * [slot_s_num];
	size_t index_of_mem = 0;
	for (size_t i = 0; i < slot_s_num; ++i)
	{
		slot_ss[i] = new(&mem[index_of_mem]) slot_s();
		slot_ss[i]->normalize_size = round_size;
		slot_ss[i]->owner = owner;
		index_of_mem += slot_s_size;
	}
}

block_s::~block_s()
{
	delete[] slot_ss;
	VirtualFree(mem, 0, MEM_RELEASE);
}

void* block_s::alloc(size_t size)
{
	assert(!is_full());
	assert(slot_ss[cur_slot_s]->valid == valid_t::slot_s_UNUSE);
	slot_ss[cur_slot_s]->valid = valid_t::slot_s_USED;
	slot_ss[cur_slot_s]->actual_size = size;
	return slot_ss[cur_slot_s++]->mem;
}

bool block_s::is_full()
{
	return cur_slot_s == slot_s_num;
}