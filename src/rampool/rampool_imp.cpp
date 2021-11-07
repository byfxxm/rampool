#include "stdafx.h"
#include "rampool.h"
#include "rampool_imp.h"
#include "block.h"
#include "slot.h"

rampool_imp::rampool_imp()
{
	size_t size = 0;
	for_each(__pools.begin(), __pools.end(), [&](auto& it)
		{
			size += GRANULARITY;
			it.initialize(size, this);
		});
}

rampool_imp::~rampool_imp()
{
	auto_gc(false);
	destroy();
}

rampool_imp* rampool_imp::instance()
{
	static rampool_imp inst;
	return &inst;
}

void rampool_imp::destroy()
{
	for (auto& pl : __pools)
		pl.destroy();
}

void* rampool_imp::malloc(size_t size)
{
	if (size == 0 || size > MAXSIZE)
		throw bad_alloc();

	return __pools[POOLINDEX(size)].malloc(size);
}

void rampool_imp::free(void* p)
{
	if (!p)
		return;

	__pools[POOLINDEX(__slot_cast(p)->normalize_size)].free(p);
}

void* rampool_imp::realloc(void* p, size_t size)
{
	if (!p)
		return malloc(size);

	auto slt = __slot_cast(p);

	if (size <= slt->normalize_size)
	{
		__pools[POOLINDEX(slt->normalize_size)].total() += size - slt->actual_size;
		slt->actual_size = size;
		return p;
	}

	auto p_ = malloc(size);
	memmove(p_, p, slt->actual_size);
	free(p);
	return p_;
}

void rampool_imp::leak(leak_info* info)
{
	if (!info)
		return;

	memset(info, 0, sizeof(leak_info));
	for (auto& pl : __pools)
	{
		info->count += pl.count();
		info->total_size += pl.count() * pl.get_size();
		info->total_actual_size += pl.total();
	}

	assert(info->total_actual_size <= info->total_size);
}

size_t rampool_imp::size(void* p)
{
	return __slot_cast(p)->actual_size;
}

void rampool_imp::gc()
{
	for (auto& pl : __pools)
		pl.gc();
}

void rampool_imp::auto_gc(bool b)
{
	__is_auto_gc = b;

	if (__is_auto_gc)
	{
		if (__auto_gc_thrd.joinable())
			return;

		__auto_gc_thrd = thread([this]()
			{
				while (__is_auto_gc)
				{
					for (auto& pl : __pools)
					{
						if (pl.need_gc())
							pl.gc();
					}

					yield();
				}
			});
	}
	else
	{
		if (__auto_gc_thrd.joinable())
			__auto_gc_thrd.join();
	}
}

inline slot* rampool_imp::__slot_cast(void* p) const
{
	auto slt = POINTER_TO_SLOT(p);

	if (slt->owner != this || slt->valid != valid_t::SLOT_USED)
		throw exception("invalid ptr");

	return slt;
}