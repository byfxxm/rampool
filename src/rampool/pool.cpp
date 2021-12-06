#include "stdafx.h"
#include "pool.h"
#include "block.h"
#include "slot.h"

void pool::initialize(size_t size, const void* owner)
{
	__size = size;
	__owner = owner;
}

size_t pool::get_size()
{
	return __size;
}

void* pool::malloc(size_t size)
{
	lock_ty lock(__mutex);
	++__count;
	__total += size;

	auto slot_ = __free_stack.pop();
	if (slot_)
	{
		assert(slot_->valid == valid_t::SLOT_DELETED);
		slot_->valid = valid_t::SLOT_USED;
		slot_->actual_size = size;
		return slot_->mem;
	}

	auto block_ = __block_stack.top();
	if (!block_ || block_->is_full())
	{
		block_ = new block(__size, __owner);
		__block_stack.push(block_);
	}

	return block_->alloc(size);
}

void pool::free(void* p)
{
	lock_ty lock(__mutex);

	auto slot_ = POINTER_TO_SLOT(p);
	slot_->valid = valid_t::SLOT_DELETED;
	__free_stack.push(slot_);
	--__count;
	__total -= slot_->actual_size;
	assert((int)__count >= 0);
}

void pool::destroy()
{
	lock_ty lock(__mutex);

	block* block_ = nullptr;
	while (block_ = __block_stack.top())
	{
		__block_stack.pop();
		delete block_;
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
	lock_ty lock(__mutex);

	block* next = nullptr;
	for (auto block_ = __block_stack.top(); block_; block_ = next)
	{
		next = block_->next;

		size_t idx = 0;
		for (; idx < block_->cur_slot; ++idx)
		{
			assert(block_->slots[idx]->valid != valid_t::SLOT_UNUSE);
			if (block_->slots[idx]->valid == valid_t::SLOT_USED)
				break;
		}

		if (idx == block_->cur_slot)
		{
			for (size_t i = 0; i < block_->cur_slot; ++i)
				block_->slots[i]->valid = valid_t::SLOT_UNUSE;

			for (auto slot_ = __free_stack.top(); slot_; slot_ = slot_->next)
			{
				if (slot_->valid == valid_t::SLOT_UNUSE)
					__free_stack.erase(slot_);
			}

			__block_stack.erase(block_);
			delete block_;
		}
	}
}

bool pool::need_gc()
{
	return __free_stack.count() >= AUTOGC_THRESHOLD;
}