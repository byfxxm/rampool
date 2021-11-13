#pragma once
#include "rampool.h"
#include "pool.h"

class rampool_imp
{
public:
	rampool_imp();
	~rampool_imp();
	static rampool_imp* instance();
	void* malloc(size_t);
	void free(void*);
	void* realloc(void*, size_t);
	void destroy();
	void leak(leak_info*);
	size_t size(void*);
	void gc();
	void auto_gc(bool);

private:
	slot* __slot_cast(void*) const;

private:
	array<pool, POOLNUM> __pools;
	thread __auto_gc_thrd;
	volatile bool __is_auto_gc = false;
};