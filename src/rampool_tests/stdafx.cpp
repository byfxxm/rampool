#include "stdafx.h"

#ifdef _WIN64
#define PLATFORM "x64"
#else
#define PLATFORM ""
#endif

#ifdef _DEBUG
#define CONFIG "Debug"
#else
#define CONFIG "Release"
#endif

#pragma comment(lib, "../" PLATFORM "/" CONFIG "/rampool.lib")
#pragma comment(lib, "../" PLATFORM "/" CONFIG "/lua.lib")
#pragma comment(lib, "DbgHelp.lib")