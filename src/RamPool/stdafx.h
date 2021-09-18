#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <Windows.h>
#include <mutex>
#include <cassert>

#define GRANULARITY 16
#define ROUND(size) (((size) + GRANULARITY - 1) & ~(GRANULARITY - 1))
#define MAXSIZE (1024 * 1024)
#define POOLNUM (MAXSIZE / GRANULARITY)
#define POOLINDEX(size) (ROUND(size) / GRANULARITY - 1)
#define POINTER_TO_SLOT(p) ((Slot*)((char*)(p) - sizeof(Slot)))

using namespace std;