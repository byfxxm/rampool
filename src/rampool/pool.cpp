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
	++__count;
	__total += size;

	auto slt = __free_stack.pop();
	if (slt) {
		assert(slt->valid == slot::valid_t::DELETED);
		slt->valid = slot::valid_t::USED;
		slt->actual_size = size;
		return slt->mem;
	}

	auto blk = __block_stack.top();
	if (!blk || blk->is_full()) {
		blk = new block(__size, __owner);
		__block_stack.push(blk);
	}

	return blk->alloc(size);
}

void pool::free(void* p) {
	lock_t lck(__mutex);

	auto slt = POINTER_TO_slot_s(p);
	slt->valid = slot::valid_t::DELETED;
	__free_stack.push(slt);
	--__count;
	__total -= slt->actual_size;
	assert((int)__count >= 0);
}

void pool::destroy() {
	lock_t lck(__mutex);

	block* blk = nullptr;
	while (blk = __block_stack.top()) {
		__block_stack.pop();
		delete blk;
	}

	new(&__block_stack) stack<block>();
	new(&__free_stack) stack<slot>();
	__count = 0;
	__total = 0;
}

size_t pool::count()
{
	return __count;
}

size_t pool::total()
{
	return __total;
}

void pool::gc()
{
	lock_t lck(__mutex);

	block* next = nullptr;
	for (auto blk = __block_stack.top(); blk; blk = next) {
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

			for (auto slot = __free_stack.top(); slot; slot = slot->next) {
				if (slot->valid == slot::valid_t::UNUSE)
					__free_stack.erase(slot);
			}

			__block_stack.erase(blk);
			delete blk;
		}
	}
}

bool pool::need_gc() {
	return __free_stack.count() >= AUTOGC_THRESHOLD;
}