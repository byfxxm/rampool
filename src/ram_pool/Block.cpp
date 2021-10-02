#include "stdafx.h"
#include "block.h"
#include "slot.h"

block::block(size_t size, void* owner)
{
	if (size <= MAXSIZE / 8)
		_slot_num = 32;
	else if (size <= MAXSIZE)
		_slot_num = 4;
	else
		assert(false);

	auto round_size = ROUND(size);
	auto slot_size = sizeof(slot) + round_size;
	_mem_size = slot_size * _slot_num;
	_mem = (char*)VirtualAlloc(nullptr, _mem_size, MEM_COMMIT, PAGE_READWRITE);
	_mem ? memset(_mem, 0, _mem_size) : throw bad_alloc();

	_slots = new slot * [_slot_num];
	auto index_of_mem = 0;
	for (size_t _i = 0; _i < _slot_num; _i++)
	{
		_slots[_i] = new(&_mem[index_of_mem]) slot();
		_slots[_i]->normalize_size = round_size;
		_slots[_i]->owner = owner;
		index_of_mem += slot_size;
	}
}

block::~block()
{
	delete[] _slots;
	VirtualFree(_mem, 0, MEM_RELEASE);
}

void* block::alloc(size_t size)
{
	assert(!is_full());
	assert(_slots[_cur_slot]->valid == valid_t::SLOT_UNUSE);
	_slots[_cur_slot]->valid = valid_t::SLOT_USED;
	_slots[_cur_slot]->actual_size = size;
	return _slots[_cur_slot++]->mem;
}

bool block::is_full()
{
	return _cur_slot == _slot_num;
}