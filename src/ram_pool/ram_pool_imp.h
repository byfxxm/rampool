#pragma once
#include "ram_pool.h"
#include "pool.h"

class ram_pool_imp
{
public:
	ram_pool_imp();
	~ram_pool_imp();
	static ram_pool_imp* instance();
	void* malloc(size_t);
	void free(void*);
	void* realloc(void*, size_t);
	void destroy();
	void leak(leak_info*);
	size_t size(void*);
	void gc();
	void auto_gc(bool);

private:
	pool _pools[POOLNUM];
	thread _auto_gc_thrd;
	atomic<bool> _is_auto_gc = false;
};