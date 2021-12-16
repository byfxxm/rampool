#include "stdafx.h"
#include "rampool.h"
#include "rampool_imp.h"

void* rp_malloc(size_t size)
{
	return rampool_imp_c::instance()->malloc(size);
}

void rp_free(void* p)
{
	return rampool_imp_c::instance()->free(p);
}

void* rp_realloc(void* p, size_t size)
{
	return rampool_imp_c::instance()->realloc(p, size);
}

void rp_destroy()
{
	return rampool_imp_c::instance()->destroy();
}

void rp_leak(leak_info* info)
{
	return rampool_imp_c::instance()->leak(info);
}

size_t rp_size(void* p)
{
	return rampool_imp_c::instance()->size(p);
}

void rp_gc()
{
	return rampool_imp_c::instance()->gc();
}

void rp_auto_gc(bool b)
{
	return rampool_imp_c::instance()->auto_gc(b);
}

void* rp_heap_create()
{
	return new rampool_imp_c();
}

void rp_heap_delete(void* heap)
{
	return delete (rampool_imp_c*)heap;
}

void* rp_heap_malloc(void* heap, size_t size)
{
	return ((rampool_imp_c*)heap)->malloc(size);
}

void rp_heap_free(void* heap, void* p)
{
	return ((rampool_imp_c*)heap)->free(p);
}

void* rp_heap_realloc(void* heap, void* p, size_t size)
{
	return ((rampool_imp_c*)heap)->realloc(p, size);
}

void rp_heap_destroy(void* heap)
{
	return ((rampool_imp_c*)heap)->destroy();
}

void rp_heap_leak(void* heap, leak_info* info)
{
	return ((rampool_imp_c*)heap)->leak(info);
}

size_t rp_heap_size(void* heap, void* p)
{
	return ((rampool_imp_c*)heap)->size(p);
}

void rp_heap_gc(void* heap)
{
	return ((rampool_imp_c*)heap)->gc();
}

void rp_heap_auto_gc(void* heap, bool b)
{
	return ((rampool_imp_c*)heap)->auto_gc(b);
}