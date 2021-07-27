#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>

#define GRANULARITY 16
#define ROUND(size) (((size) + GRANULARITY - 1) & ~(GRANULARITY - 1))
#define MAXSIZE 10240
#define POOLNUM (MAXSIZE / GRANULARITY)
#define POOLINDEX(size) (ROUND(size) / GRANULARITY - 1)
#define SLOTNUM 64