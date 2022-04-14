#include "stdafx.h"
#include "rampool.h"
#include "rampool_imp.h"

void* rp_malloc(size_t Size) {
	return RampoolImp::Instance()->Malloc(Size);
}

void rp_free(void* p) {
	return RampoolImp::Instance()->Free(p);
}

void* rp_realloc(void* p, size_t Size) {
	return RampoolImp::Instance()->Realloc(p, Size);
}

void rp_destroy() {
	return RampoolImp::Instance()->Destroy();
}

void rp_leak(LeakInfo* info) {
	return RampoolImp::Instance()->Leak(info);
}

size_t rp_size(void* p) {
	return RampoolImp::Instance()->Size(p);
}

void rp_gc() {
	return RampoolImp::Instance()->Gc();
}

void rp_auto_gc(bool b) {
	return RampoolImp::Instance()->AutoGc(b);
}

void* rp_heap_create() {
	return new RampoolImp();
}

void rp_heap_delete(void* heap) {
	return delete (RampoolImp*)heap;
}

void* rp_heap_malloc(void* heap, size_t Size) {
	return ((RampoolImp*)heap)->Malloc(Size);
}

void rp_heap_free(void* heap, void* p) {
	return ((RampoolImp*)heap)->Free(p);
}

void* rp_heap_realloc(void* heap, void* p, size_t Size) {
	return ((RampoolImp*)heap)->Realloc(p, Size);
}

void rp_heap_destroy(void* heap) {
	return ((RampoolImp*)heap)->Destroy();
}

void rp_heap_leak(void* heap, LeakInfo* info) {
	return ((RampoolImp*)heap)->Leak(info);
}

size_t rp_heap_size(void* heap, void* p) {
	return ((RampoolImp*)heap)->Size(p);
}

void rp_heap_gc(void* heap) {
	return ((RampoolImp*)heap)->Gc();
}

void rp_heap_auto_gc(void* heap, bool b) {
	return ((RampoolImp*)heap)->AutoGc(b);
}