#pragma once
#include "stack.h"
#include "block.h"
#include "slot.h"

class pool
{
public:
	pool() = default;
	~pool() = default;
	void set_size(size_t);
	size_t get_size();
	void* malloc(size_t);
	void free(void*);
	void destroy();
	size_t count();
	atomic<size_t>& total();
	void gc();
	bool need_gc();

private:
	mutex _mtx;
	stack<block> _block_stack;
	stack<slot> _free_stack;
	atomic<size_t> _size = 0;
	atomic<size_t> _count = 0;
	atomic<size_t> _total = 0;
};