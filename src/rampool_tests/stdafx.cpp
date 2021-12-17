#include "stdafx.h"

#ifdef _WIN64
#define PLATFORM "x64"
#else
#define PLATFORM ""
#endif
#ifdef _DEBUG
#define CONFIGURATION "Debug"
#else
#define CONFIGURATION "Release"
#endif
#define LIB_DIR "../" PLATFORM "/" CONFIGURATION "/"

#pragma comment(lib, LIB_DIR "rampool.lib")
#pragma comment(lib, LIB_DIR "lua.lib")
#pragma comment(lib, "DbgHelp.lib")