#include "stdafx.h"
#include "pool.h"
#include "block.h"
#include "slot.h"

void pool::set_size(size_t size)
{
	_size = size;
}

size_t pool::get_size()
{
	return _size;
}

void* pool::malloc(size_t size)
{
	unique_lock<mutex> lck(_mtx);
	_count++;
	_total += size;

	auto slt = _free_stack.top();
	if (slt)
	{
		_free_stack.pop();
		assert(slt->valid == valid_t::SLOT_DELETED);
		slt->valid = valid_t::SLOT_USED;
		slt->actual_size = size;
		return slt->mem;
	}

	auto blk = _block_stack.top();
	if (!blk || blk->is_full())
	{
		blk = new block(_size, this);
		_block_stack.push(blk);
	}

	return blk->alloc(size);
}

void pool::free(void* p)
{
	unique_lock<mutex> lck(_mtx);

	auto slt = POINTER_TO_SLOT(p);
	assert(slt->owner == this);
	assert(slt->valid == valid_t::SLOT_USED);
	slt->valid = valid_t::SLOT_DELETED;

	_free_stack.push(slt);
	_count--;
	_total -= slt->actual_size;
	assert((int)_count >= 0);
}

void pool::destroy()
{
	unique_lock<mutex> lck(_mtx);

	block* blk = nullptr;
	while (blk = _block_stack.top())
	{
		_block_stack.pop();
		delete blk;
	}

	new(&_block_stack) stack<block>();
	new(&_free_stack) stack<slot>();
	_count = 0;
	_total = 0;
}

size_t pool::count()
{
	return _count;
}

atomic<size_t>& pool::total()
{
	return _total;
}

void pool::gc()
{
	unique_lock<mutex> lck(_mtx);

	block* next = NULL;
	for (auto blk = _block_stack.top(); blk; blk = next)
	{
		next = blk->next;

		size_t idx = 0;
		for (; idx < blk->cur_slot; idx++)
		{
			assert(blk->slots[idx]->valid != valid_t::SLOT_UNUSE);
			if (blk->slots[idx]->valid == valid_t::SLOT_USED)
				break;
		}

		if (idx == blk->cur_slot)
		{
			for (size_t i = 0; i < blk->cur_slot; i++)
				blk->slots[i]->valid = valid_t::SLOT_UNUSE;

			for (auto slt = _free_stack.top(); slt; slt = slt->next)
			{
				if (slt->valid == valid_t::SLOT_UNUSE)
					_free_stack.erase(slt);
			}

			_block_stack.erase(blk);
			delete blk;
		}
	}
}

bool pool::need_gc()
{
	return _free_stack.count() >= AUTOGC_THRESHOLD;
}