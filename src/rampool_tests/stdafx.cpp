#include "stdafx.h"

#ifdef _WIN64
#define PLATFORM "x64"
#else
#define PLATFORM
#endif
#ifdef _DEBUG
#define CONFIG "Debug"
#else
#define CONFIG "Release"
#endif
#define LIB_DIR "../" PLATFORM "/" CONFIG "/"

#pragma comment(lib, LIB_DIR "rampool.lib")
#pragma comment(lib, LIB_DIR "lua.lib")
#pragma comment(lib, "DbgHelp.lib")