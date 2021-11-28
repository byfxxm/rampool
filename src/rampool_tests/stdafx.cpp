#include "stdafx.h"

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment(lib, "../x64/Debug/rampool.lib")
		#pragma comment(lib, "../x64/Debug/lua.lib")
	#else
		#pragma comment(lib, "../x64/Release/rampool.lib")
		#pragma comment(lib, "../x64/Release/lua.lib")
	#endif
#else
	#ifdef _DEBUG
		#pragma comment(lib, "../Debug/rampool.lib")
		#pragma comment(lib, "../Debug/lua.lib")
	#else
		#pragma comment(lib, "../Release/rampool.lib")
		#pragma comment(lib, "../Release/lua.lib")
	#endif
#endif

#pragma comment(lib, "DbgHelp.lib")