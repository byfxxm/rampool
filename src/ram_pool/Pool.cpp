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

	auto _pSlot = _free_stack.Top();
	if (_pSlot)
	{
		_free_stack.Pop();
		assert(_pSlot->valid == valid_t::SLOT_DELETED);
		_pSlot->valid = valid_t::SLOT_USED;
		_pSlot->actual_size = size;
		return _pSlot->mem;
	}

	auto _pBlock = _block_stack.Top();
	if (!_pBlock || _pBlock->is_full())
	{
		_pBlock = new block(_size, this);
		_block_stack.Push(_pBlock);
	}

	return _pBlock->alloc(size);
}

void pool::free(void* p)
{
	unique_lock<mutex> lck(_mtx);

	auto _pSlot = POINTER_TO_SLOT(p);
	assert(_pSlot->owner == this);
	assert(_pSlot->valid == valid_t::SLOT_USED);
	_pSlot->valid = valid_t::SLOT_DELETED;

	_free_stack.Push(_pSlot);
	_count--;
	_total -= _pSlot->actual_size;
	assert((int)_count >= 0);
}

void pool::destroy()
{
	unique_lock<mutex> lck(_mtx);

	block* _p = nullptr;
	while (_p = _block_stack.Top())
	{
		_block_stack.Pop();
		delete _p;
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
	for (auto bl = _block_stack.Top(); bl; bl = next)
	{
		next = bl->m_pNext;

		size_t ind = 0;
		for (; ind < bl->_cur_slot; ind++)
		{
			assert(bl->_slots[ind]->valid != valid_t::SLOT_UNUSE);
			if (bl->_slots[ind]->valid == valid_t::SLOT_USED)
				break;
		}

		if (ind == bl->_cur_slot)
		{
			for (size_t _i = 0; _i < bl->_cur_slot; _i++)
				bl->_slots[_i]->valid = valid_t::SLOT_UNUSE;

			for (auto _pSlot = _free_stack.Top(); _pSlot; _pSlot = _pSlot->m_pNext)
			{
				if (_pSlot->valid == valid_t::SLOT_UNUSE)
					_free_stack.Erase(_pSlot);
			}

			_block_stack.Erase(bl);
			delete bl;
		}
	}
}

bool pool::need_gc()
{
	return _free_stack.Count() >= AUTOGC_THRESHOLD;
}