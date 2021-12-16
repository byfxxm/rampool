#include "stdafx.h"
#include "rampool_imp.h"
#include "block.h"
#include "slot.h"

rampool_imp_c::rampool_imp_c()
{
	size_t size = 0;
	for_each(__pools.begin(), __pools.end(), [&](auto& it)
		{
			size += GRANULARITY;
			it.initialize(size, this);
		});
}

rampool_imp_c::~rampool_imp_c()
{
	auto_gc(false);
	destroy();
}

rampool_imp_c* rampool_imp_c::instance()
{
	static rampool_imp_c inst;
	return &inst;
}

void rampool_imp_c::destroy()
{
	for (auto& pool_ : __pools)
		pool_.destroy();
}

void* rampool_imp_c::malloc(size_t size)
{
	if (size == 0 || size > MAXSIZE)
		return nullptr;

	return __pools[POOLINDEX(size)].malloc(size);
}

void rampool_imp_c::free(void* p)
{
	if (!p)
		return;

	__pools[POOLINDEX(__slot_cast(p)->normalize_size)].free(p);
}

void* rampool_imp_c::realloc(void* p, size_t size)
{
	if (!p)
		return malloc(size);

	auto slot_s_ = __slot_cast(p);
	auto p_ = malloc(size);
	memmove(p_, p, min(slot_s_->actual_size, size));
	free(p);
	return p_;
}

void rampool_imp_c::leak(leak_info_s* info)
{
	if (!info)
		return;

	memset(info, 0, sizeof(leak_info_s));
	for (auto& pool_ : __pools)
	{
		info->count += pool_.count();
		info->total_size += pool_.count() * pool_.get_size();
		info->total_actual_size += pool_.total();
	}

	assert(info->total_actual_size <= info->total_size);
}

size_t rampool_imp_c::size(void* p)
{
	return __slot_cast(p)->actual_size;
}

void rampool_imp_c::gc()
{
	for (auto& pool_ : __pools)
		pool_.gc();
}

void rampool_imp_c::auto_gc(bool b)
{
	__is_auto_gc = b;

	if (__is_auto_gc)
	{
		if (__auto_gc_thrd.joinable())
			return;

		__auto_gc_thrd = std::thread([this]()
			{
				while (__is_auto_gc)
				{
					for (auto& pool_ : __pools)
					{
						if (pool_.need_gc())
							pool_.gc();
					}

					std::this_thread::yield();
				}
			});
	}
	else
	{
		if (__auto_gc_thrd.joinable())
			__auto_gc_thrd.join();
	}
}

inline slot_s* rampool_imp_c::__slot_cast(void* p) const
{
	auto slot_s_ = POINTER_TO_slot_s(p);

	if (slot_s_->owner != this || slot_s_->valid != valid_t::SLOT_USED)
		throw std::exception("invalid ptr");

	return slot_s_;
}