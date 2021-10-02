#pragma once
#include "stack.h"
#include "block.h"
#include "slot.h"

class pool
{
public:
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
	mutex __mtx;
	stack<block> __block_stack;
	stack<slot> __free_stack;
	atomic<size_t> __size = 0;
	atomic<size_t> __count = 0;
	atomic<size_t> __total = 0;
};