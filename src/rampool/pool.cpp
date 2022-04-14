#include "stdafx.h"
#include "pool.h"
#include "block.h"
#include "Slot.h"

void pool::initialize(size_t Size, const void* owner) {
	__size = Size;
	__owner = owner;
}

size_t pool::get_size() {
	return __size;
}

void* pool::Malloc(size_t Size) {
	lock_t lck(__mutex);
	++count_;
	__total += Size;

	auto slt = __free_Stack.pop();
	if (slt) {
		assert(slt->valid == Slot::Valid::DELETED);
		slt->valid = Slot::Valid::USED;
		slt->actual_size = Size;
		return slt->mem;
	}

	auto blk = __block_Stack.top();
	if (!blk || blk->is_full()) {
		blk = new block(__size, __owner);
		__block_Stack.push(blk);
	}

	return blk->alloc(Size);
}

void pool::Free(void* p) {
	lock_t lck(__mutex);

	auto slt = POINTER_TO_slot_s(p);
	slt->valid = Slot::Valid::DELETED;
	__free_Stack.push(slt);
	--count_;
	__total -= slt->actual_size;
	assert((int)count_ >= 0);
}

void pool::Destroy() {
	lock_t lck(__mutex);

	block* blk = nullptr;
	while (blk = __block_Stack.top()) {
		__block_Stack.pop();
		delete blk;
	}

	new(&__block_Stack) Stack<block>();
	new(&__free_Stack) Stack<Slot>();
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

void pool::Gc()
{
	lock_t lck(__mutex);

	block* next = nullptr;
	for (auto blk = __block_Stack.top(); blk; blk = next) {
		next = blk->next;

		size_t index = 0;
		for (; index < blk->cur_slot; ++index) {
			assert(block_->slots[index]->valid != Slot::Valid::UNUSE);
			if (blk->slots[index]->valid == Slot::Valid::USED)
				break;
		}

		if (index == blk->cur_slot) {
			for (size_t i = 0; i < blk->cur_slot; ++i)
				blk->slots[i]->valid = Slot::Valid::UNUSE;

			for (auto Slot = __free_Stack.top(); Slot; Slot = Slot->next) {
				if (Slot->valid == Slot::Valid::UNUSE)
					__free_Stack.erase(Slot);
			}

			__block_Stack.erase(blk);
			delete blk;
		}
	}
}

bool pool::need_gc() {
	return __free_Stack.count() >= AUTOGC_THRESHOLD;
}