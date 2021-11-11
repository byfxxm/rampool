#pragma once
#include "spin.h"

struct leak_info;
class rampool_imp;

class rampool_mt
{
private:
	using thread_id = unsigned long;

public:
	rampool_mt();
	~rampool_mt();
	static rampool_mt& instance();
	void* malloc(size_t);
	void free(void*);
	void* realloc(void*, size_t);
	void leak(leak_info*);
	size_t size(void*);
	void gc();

private:
	void __gc();

private:
	thread gc_thd;
	atomic<bool> is_exitting = false;
	condition_variable cv;
	mutex mtx;
	list<void*> free_deferred_list;
};