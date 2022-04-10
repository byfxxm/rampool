#include "stdafx.h"
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
	for (auto& po : __pools)
		po.destroy();
}

void* rampool_imp::malloc(size_t size)
{
	if (size == 0 || size > MAXSIZE)
		return nullptr;

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
	auto p_ = malloc(size);
	memmove(p_, p, min(slt->actual_size, size));
	free(p);
	return p_;
}

void rampool_imp::leak(leak_info_s* info)
{
	if (!info)
		return;

	memset(info, 0, sizeof(leak_info_s));
	for (auto& po : __pools)
	{
		info->count += po.count();
		info->total_size += po.count() * po.get_size();
		info->total_actual_size += po.total();
	}

	assert(info->total_actual_size <= info->total_size);
}

size_t rampool_imp::size(void* p)
{
	return __slot_cast(p)->actual_size;
}

void rampool_imp::gc()
{
	for (auto& pool : __pools)
		pool.gc();
}

void rampool_imp::auto_gc(bool b)
{
	__is_auto_gc = b;

	if (__is_auto_gc)
	{
		if (__auto_gc_thread.joinable())
			return;

		__auto_gc_thread = std::thread([this]()
			{
				while (__is_auto_gc)
				{
					for (auto& po : __pools)
					{
						if (po.need_gc())
							po.gc();
					}

					std::this_thread::yield();
				}
			});
	}
	else
	{
		if (__auto_gc_thread.joinable())
			__auto_gc_thread.join();
	}
}

inline slot* rampool_imp::__slot_cast(void* p) const
{
	auto slt = POINTER_TO_slot_s(p);

	if (slt->owner != this || slt->valid != slot::valid_t::USED)
		throw std::exception("invalid ptr");

	return slt;
}