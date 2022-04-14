#pragma once

#ifdef RAMPOOL_EXPORTS
#define RAMPOOL_API __declspec(dllexport)
#else
#define RAMPOOL_API __declspec(dllimport)
#endif

struct LeakInfo {
	size_t count;
	size_t total_size;
	size_t total_actual_size;
};

#ifdef __cplusplus
extern "C" {
#endif
	RAMPOOL_API void* rp_malloc(size_t Size);
	RAMPOOL_API void rp_free(void* p);
	RAMPOOL_API void* rp_realloc(void* p, size_t Size);
	RAMPOOL_API void rp_destroy();
	RAMPOOL_API void rp_leak(LeakInfo* info);
	RAMPOOL_API size_t rp_size(void* p);
	RAMPOOL_API void rp_gc();
	RAMPOOL_API void rp_auto_gc(bool b);

	RAMPOOL_API void* rp_heap_create();
	RAMPOOL_API void rp_heap_delete(void* heap);
	RAMPOOL_API void* rp_heap_malloc(void* heap, size_t Size);
	RAMPOOL_API void rp_heap_free(void* heap, void* p);
	RAMPOOL_API void* rp_heap_realloc(void* heap, void* p, size_t Size);
	RAMPOOL_API void rp_heap_destroy(void* heap);
	RAMPOOL_API void rp_heap_leak(void* heap, LeakInfo* info);
	RAMPOOL_API size_t rp_heap_size(void* heap, void* p);
	RAMPOOL_API void rp_heap_gc(void* heap);
	RAMPOOL_API void rp_heap_auto_gc(void* heap, bool b);
#ifdef __cplusplus
}
#endif