#include "stdafx.h"
#include "block.h"
#include "Slot.h"

block::block(size_t Size, const void* owner) {
	if (Size <= MAXSIZE / 8)
		slot_num = 32;
	else if (Size <= MAXSIZE)
		slot_num = 4;
	else
		assert(false);

	auto round_size = ROUND(Size);
	auto slot_size = sizeof(Slot) + round_size;
	mem_size = slot_size * slot_num;
	mem = (char*)VirtualAlloc(nullptr, mem_size, MEM_COMMIT, PAGE_READWRITE);
	mem ? memset(mem, 0, mem_size) : throw std::bad_alloc();

	slots = new Slot * [slot_num];
	size_t index_of_mem = 0;
	for (size_t i = 0; i < slot_num; ++i) {
		slots[i] = new(&mem[index_of_mem]) Slot();
		slots[i]->normalize_size = round_size;
		slots[i]->owner = owner;
		index_of_mem += slot_size;
	}
}

block::~block() {
	delete[] slots;
	VirtualFree(mem, 0, MEM_RELEASE);
}

void* block::alloc(size_t Size) {
	assert(!is_full());
	assert(slots[cur_slot]->valid == Slot::Valid::UNUSE);
	slots[cur_slot]->valid = Slot::Valid::USED;
	slots[cur_slot]->actual_size = Size;
	return slots[cur_slot++]->mem;
}

bool block::is_full() {
	return cur_slot == slot_num;
}