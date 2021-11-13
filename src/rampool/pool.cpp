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
	lock_ty lck(__mtx);
	++__count;
	__total += size;

	auto slt = __free_stack.top();
	if (slt)
	{
		__free_stack.pop();
		assert(slt->valid == valid_t::SLOT_DELETED);
		slt->valid = valid_t::SLOT_USED;
		slt->actual_size = size;
		return slt->mem;
	}

	auto blk = __block_stack.top();
	if (!blk || blk->is_full())
	{
		blk = new block(__size, __owner);
		__block_stack.push(blk);
	}

	return blk->alloc(size);
}

void pool::free(void* p)
{
	lock_ty lck(__mtx);

	auto slt = POINTER_TO_SLOT(p);
	slt->valid = valid_t::SLOT_DELETED;
	__free_stack.push(slt);

	--__count;
	__total -= slt->actual_size;
	assert((int)__count >= 0);
}

void pool::destroy()
{
	lock_ty lck(__mtx);

	block* blk = nullptr;
	while (blk = __block_stack.top())
	{
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
	lock_ty lck(__mtx);

	block* next = nullptr;
	for (auto blk = __block_stack.top(); blk; blk = next)
	{
		next = blk->next;

		size_t idx = 0;
		for (; idx < blk->cur_slot; ++idx)
		{
			assert(blk->slots[idx]->valid != valid_t::SLOT_UNUSE);
			if (blk->slots[idx]->valid == valid_t::SLOT_USED)
				break;
		}

		if (idx == blk->cur_slot)
		{
			for (size_t i = 0; i < blk->cur_slot; ++i)
				blk->slots[i]->valid = valid_t::SLOT_UNUSE;

			for (auto slt = __free_stack.top(); slt; slt = slt->next)
			{
				if (slt->valid == valid_t::SLOT_UNUSE)
					__free_stack.erase(slt);
			}

			__block_stack.erase(blk);
			delete blk;
		}
	}
}

bool pool::need_gc()
{
	return __free_stack.count() >= AUTOGC_THRESHOLD;
}