#include "stdafx.h"
#include "ram_pool.h"
#include "ram_pool_imp.h"

void* rp_heap_create()
{
	return new ram_pool_imp();
}

void rp_heap_delete(void* heap)
{
	return delete (ram_pool_imp*)heap;
}

void* rp_heap_malloc(void* heap, size_t size)
{
	return ((ram_pool_imp*)heap)->malloc(size);
}

void rp_heap_free(void* heap, void* p)
{
	return ((ram_pool_imp*)heap)->free(p);
}

void* rp_heap_realloc(void* heap, void* p, size_t size)
{
	return ((ram_pool_imp*)heap)->realloc(p, size);
}

void rp_heap_destroy(void* heap)
{
	return ((ram_pool_imp*)heap)->destroy();
}

void rp_heap_leak(void* heap, leak_info* info)
{
	return ((ram_pool_imp*)heap)->leak(info);
}

size_t rp_heap_size(void* heap, void* p)
{
	return ((ram_pool_imp*)heap)->size(p);
}

void rp_heap_gc(void* heap)
{
	return ((ram_pool_imp*)heap)->gc();
}

void rp_heap_auto_gc(void* heap, bool b)
{
	return ((ram_pool_imp*)heap)->auto_gc(b);
}

void* rp_malloc(size_t size)
{
	return ram_pool_imp::instance()->malloc(size);
}

void rp_free(void* p)
{
	return ram_pool_imp::instance()->free(p);
}

void* rp_realloc(void* p, size_t size)
{
	return ram_pool_imp::instance()->realloc(p, size);
}

void rp_destroy()
{
	return ram_pool_imp::instance()->destroy();
}

void rp_leak(leak_info* info)
{
	return ram_pool_imp::instance()->leak(info);
}

size_t rp_size(void* p)
{
	return ram_pool_imp::instance()->size(p);
}

void rp_gc()
{
	return ram_pool_imp::instance()->gc();
}

void rp_auto_gc(bool b)
{
	return ram_pool_imp::instance()->auto_gc(b);
}