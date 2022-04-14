#include "stdafx.h"
#include "pool.h"
#include "block.h"
#include "slot.h"

void pool::initialize(size_t size, const void* owner) {
	__size = size;
	__owner = owner;
}

size_t pool::get_size() {
	return __size;
}

void* pool::malloc(size_t size) {
	lock_t lck(__mutex);
	++count_;
	__total += size;

	auto slt = __free_Stack.pop();
	if (slt) {
		assert(slt->valid == slot::valid_t::DELETED);
		slt->valid = slot::valid_t::USED;
		slt->actual_size = size;
		return slt->mem;
	}

	auto blk = __block_Stack.top();
	if (!blk || blk->is_full()) {
		blk = new block(__size, __owner);
		__block_Stack.push(blk);
	}

	return blk->alloc(size);
}

void pool::free(void* p) {
	lock_t lck(__mutex);

	auto slt = POINTER_TO_slot_s(p);
	slt->valid = slot::valid_t::DELETED;
	__free_Stack.push(slt);
	--count_;
	__total -= slt->actual_size;
	assert((int)count_ >= 0);
}

void pool::destroy() {
	lock_t lck(__mutex);

	block* blk = nullptr;
	while (blk = __block_Stack.top()) {
		__block_Stack.pop();
		delete blk;
	}

	new(&__block_Stack) Stack<block>();
	new(&__free_Stack) Stack<slot>();
	count_ = 0;
	__total = 0;
}

size_t pool::count()
{
	return count_;
}

size_t pool::total()
{
	return __total;
}

void pool::gc()
{
	lock_t lck(__mutex);

	block* next = nullptr;
	for (auto blk = __block_Stack.top(); blk; blk = next) {
		next = blk->next;

		size_t index = 0;
		for (; index < blk->cur_slot; ++index) {
			assert(block_->slots[index]->valid != slot::valid_t::UNUSE);
			if (blk->slots[index]->valid == slot::valid_t::USED)
				break;
		}

		if (index == blk->cur_slot) {
			for (size_t i = 0; i < blk->cur_slot; ++i)
				blk->slots[i]->valid = slot::valid_t::UNUSE;

			for (auto slot = __free_Stack.top(); slot; slot = slot->next) {
				if (slot->valid == slot::valid_t::UNUSE)
					__free_Stack.erase(slot);
			}

			__block_Stack.erase(blk);
			delete blk;
		}
	}
}

bool pool::need_gc() {
	return __free_Stack.count() >= AUTOGC_THRESHOLD;
}