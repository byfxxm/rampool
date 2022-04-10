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
	lock_t lock(__mutex);
	++__count;
	__total += size;

	auto slot = __free_stack.pop();
	if (slot)
	{
		assert(slot->valid == slot::valid_t::DELETED);
		slot->valid = slot::valid_t::USED;
		slot->actual_size = size;
		return slot->mem;
	}

	auto block = __block_stack.top();
	if (!block || block->is_full())
	{
		block = new block_s(__size, __owner);
		__block_stack.push(block);
	}

	return block->alloc(size);
}

void pool::free(void* p)
{
	lock_t lock(__mutex);

	auto slot_ = POINTER_TO_slot_s(p);
	slot_->valid = slot::valid_t::DELETED;
	__free_stack.push(slot_);
	--__count;
	__total -= slot_->actual_size;
	assert((int)__count >= 0);
}

void pool::destroy()
{
	lock_t lock(__mutex);

	block_s* block = nullptr;
	while (block = __block_stack.top())
	{
		__block_stack.pop();
		delete block;
	}

	new(&__block_stack) stack<block_s>();
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
	lock_t lock(__mutex);

	block_s* next = nullptr;
	for (auto block = __block_stack.top(); block; block = next)
	{
		next = block->next;

		size_t index = 0;
		for (; index < block->cur_slot; ++index)
		{
			assert(block->slots[index]->valid != slot::valid_t::UNUSE);
			if (block->slots[index]->valid == slot::valid_t::USED)
				break;
		}

		if (index == block->cur_slot)
		{
			for (size_t i = 0; i < block->cur_slot; ++i)
				block->slots[i]->valid = slot::valid_t::UNUSE;

			for (auto slot = __free_stack.top(); slot; slot = slot->next)
			{
				if (slot->valid == slot::valid_t::UNUSE)
					__free_stack.erase(slot);
			}

			__block_stack.erase(block);
			delete block;
		}
	}
}

bool pool::need_gc()
{
	return __free_stack.count() >= AUTOGC_THRESHOLD;
}