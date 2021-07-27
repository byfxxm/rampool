#include "stdafx.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/RamPool.lib")
#else
#pragma comment(lib, "../x64/Release/RamPool.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "../Debug/RamPool.lib")
#else
#pragma comment(lib, "../Release/RamPool.lib")
#endif
#endif