#include "stdafx.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/rampool.lib")
#pragma comment(lib, "../Debug/lua.lib")
#else
#pragma comment(lib, "../Release/rampool.lib")
#pragma comment(lib, "../Release/lua.lib")
#endif

#pragma comment(lib, "DbgHelp.lib")