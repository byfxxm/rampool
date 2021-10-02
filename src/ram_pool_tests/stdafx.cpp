#include "stdafx.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/ram_pool.lib")
#pragma comment(lib, "../Debug/lua.lib")
#else
#pragma comment(lib, "../Release/ram_pool.lib")
#pragma comment(lib, "../Release/lua.lib")
#endif

#pragma comment(lib, "DbgHelp.lib")