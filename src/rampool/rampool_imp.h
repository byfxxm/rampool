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
	pool __pools[POOLNUM];
	thread __auto_gc_thrd;
	atomic<bool> __is_auto_gc = false;
};