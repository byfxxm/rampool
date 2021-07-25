#ifdef RAMPOOL_EXPORTS
#define RAMPOOL_API extern "C" __declspec(dllexport)
#else
#define RAMPOOL_API extern "C" __declspec(dllimport)
#endif

RAMPOOL_API void* RamPool_Create();
RAMPOOL_API void RamPool_Delete(void* pRamPool_);
RAMPOOL_API void* RamPool_Malloc(void* pRamPool_, size_t nSize_);
RAMPOOL_API void RamPool_Free(void* pRamPool_, void* p_);
