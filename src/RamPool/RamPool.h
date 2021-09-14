#pragma once

#ifdef	__cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C 
#endif

#ifdef RAMPOOL_EXPORTS
#define RAMPOOL_API EXTERN_C __declspec(dllexport)
#else
#define RAMPOOL_API EXTERN_C __declspec(dllimport)
#endif

struct LeakInfo
{
	size_t nCount;
	size_t nTotalSize;
	size_t nTotalActualSize;
};

RAMPOOL_API void* RamPool_Create();
RAMPOOL_API void RamPool_Delete(void* pRamPool_);
RAMPOOL_API void* RamPool_Malloc(void* pRamPool_, size_t nSize_);
RAMPOOL_API void RamPool_Free(void* pRamPool_, void* p_);
RAMPOOL_API void* RamPool_Realloc(void* pRamPool_, void* p_, size_t nSize_);
RAMPOOL_API void RamPool_Destroy(void* pRamPool_);
RAMPOOL_API void RamPool_Leak(void* pRamPool_, LeakInfo* pLeakInfo_);
RAMPOOL_API size_t RamPool_Size(void* pRamPool_, void* p_);

RAMPOOL_API void* rp_malloc(size_t nSize_);
RAMPOOL_API void rp_free(void* p_);
RAMPOOL_API void* rp_realloc(void* p_, size_t nSize_);
RAMPOOL_API void rp_destroy();
RAMPOOL_API void rp_leak(LeakInfo* pLeakInfo_);
RAMPOOL_API size_t rp_size(void* p_);