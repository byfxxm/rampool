#pragma once
#include "stack.h"
#include "block.h"
#include "slot.h"
#include "spin.h"

class pool
{
private:
	using mutex_ty = spin;
	using lock_ty = unique_lock<mutex_ty>;

public:
	void initialize(size_t, const void*);
	size_t get_size();
	void* malloc(size_t);
	void free(void*);
	void destroy();
	size_t count();
	atomic<size_t>& total();
	void gc();
	bool need_gc();

private:
	mutex_ty __mtx;
	stack<block> __block_stack;
	stack<slot> __free_stack;
	atomic<size_t> __size = 0;
	atomic<size_t> __count = 0;
	atomic<size_t> __total = 0;
	const void* __owner = nullptr;
};