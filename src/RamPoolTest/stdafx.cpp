#include "stdafx.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/RamPool.lib")
#pragma comment(lib, "../x64/Debug/Lua.lib")
#else
#pragma comment(lib, "../x64/Release/RamPool.lib")
#pragma comment(lib, "../x64/Release/Lua.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "../Debug/RamPool.lib")
#pragma comment(lib, "../Debug/Lua.lib")
#else
#pragma comment(lib, "../Release/RamPool.lib")
#pragma comment(lib, "../Release/Lua.lib")
#endif
#endif

#pragma comment(lib, "DbgHelp.lib")