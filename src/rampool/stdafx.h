#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cassert>
#include <mutex>
#include <atomic>
#include <array>
#include <algorithm>
#include <unordered_map>

#define GRANULARITY 16
#define ROUND(size) (((size) + GRANULARITY - 1) & ~(GRANULARITY - 1))
#define MAXSIZE (1024 * 1024)
#define POOLNUM (MAXSIZE / GRANULARITY)
#define POOLINDEX(size) (ROUND(size) / GRANULARITY - 1)
#define POINTER_TO_SLOT(p) ((slot*)((char*)(p) - sizeof(slot)))
#define AUTOGC_THRESHOLD 64
#define THROW_EXCEPTION_INVALID_PTR throw exception("invalid ptr")

using namespace std;
using namespace std::this_thread;