#pragma once

#ifdef RAMPOOL_EXPORTS
#define RAMPOOL_API __declspec(dllexport)
#else
#define RAMPOOL_API __declspec(dllimport)
#endif

#ifdef	__cplusplus
extern "C"
{
#endif

	struct leak_info
	{
		size_t count;
		size_t total_size;
		size_t total_actual_size;
	};

	RAMPOOL_API void* rp_heap_create();
	RAMPOOL_API void rp_heap_delete(void*);
	RAMPOOL_API void* rp_heap_malloc(void*, size_t);
	RAMPOOL_API void rp_heap_free(void*, void*);
	RAMPOOL_API void* rp_heap_realloc(void*, void*, size_t);
	RAMPOOL_API void rp_heap_destroy(void*);
	RAMPOOL_API void rp_heap_leak(void*, leak_info*);
	RAMPOOL_API size_t rp_heap_size(void*, void*);
	RAMPOOL_API void rp_heap_gc(void*);
	RAMPOOL_API void rp_heap_auto_gc(void*, bool);

	RAMPOOL_API void* rp_malloc(size_t);
	RAMPOOL_API void rp_free(void*);
	RAMPOOL_API void* rp_realloc(void*, size_t);
	RAMPOOL_API void rp_destroy();
	RAMPOOL_API void rp_leak(leak_info*);
	RAMPOOL_API size_t rp_size(void*);
	RAMPOOL_API void rp_gc();
	RAMPOOL_API void rp_auto_gc(bool);

#ifdef __cplusplus
}
#endif