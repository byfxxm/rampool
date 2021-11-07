#include "stdafx.h"
#include "rampool_mt.h"

void* rampool_mt::malloc(size_t)
{
	return 0;
}

void rampool_mt::free(void*)
{

}

void* rampool_mt::realloc(void*, size_t)
{
	return 0;
}

void rampool_mt::leak(leak_info*)
{

}

size_t rampool_mt::size(void*)
{
	return 0;
}