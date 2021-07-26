#ifdef RAMPOOL_EXPORTS
#define RAMPOOL_API extern "C" __declspec(dllexport)
#else
#define RAMPOOL_API extern "C" __declspec(dllimport)
#endif

RAMPOOL_API void* RamPool_Create();
RAMPOOL_API void RamPool_Delete(void* pRamPool_);
RAMPOOL_API void* RamPool_Malloc(size_t nSize_, void* pRamPool_ = nullptr);
RAMPOOL_API void RamPool_Free(void* p_, void* pRamPool_ = nullptr);
RAMPOOL_API void RamPool_Destroy(void* pRamPool_ = nullptr);
