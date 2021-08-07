#pragma once

#ifdef RAMPOOL_EXPORTS
#define RAMPOOL_API extern "C" __declspec(dllexport)
#else
#define RAMPOOL_API extern "C" __declspec(dllimport)
#endif

RAMPOOL_API void* RamPool_Create();
RAMPOOL_API void RamPool_Delete(void* pRamPool_);
RAMPOOL_API void* RamPool_Malloc(void* pRamPool_, size_t nSize_);
RAMPOOL_API void RamPool_Free(void* pRamPool_, void* p_);
RAMPOOL_API void RamPool_Destroy(void* pRamPool_);
RAMPOOL_API size_t RamPool_Leak(void* pRamPool_);

RAMPOOL_API void* rp_malloc(size_t nSize_);
RAMPOOL_API void rp_free(void* p_);
RAMPOOL_API void rp_destroy();
RAMPOOL_API size_t rp_leak();