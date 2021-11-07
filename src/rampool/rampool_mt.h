#pragma once

struct leak_info;
class rampool_imp;

class rampool_mt
{
public:
	void* malloc(size_t);
	void free(void*);
	void* realloc(void*, size_t);
	void leak(leak_info*);
	size_t size(void*);

private:
	unordered_map<size_t, rampool_imp*> thd_rp_map;
};