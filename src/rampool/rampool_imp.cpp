#include "stdafx.h"
#include "rampool_imp.h"
#include "block.h"
#include "slot.h"

rampool_imp::rampool_imp()
{
	size_t size = 0;
	for (size_t i = 0; i < _countof(_pools); i++)
	{
		size += GRANULARITY;
		_pools[i].set_size(size);
	}
}

rampool_imp::~rampool_imp()
{
	destroy();
}

rampool_imp* rampool_imp::instance()
{
	static rampool_imp inst;
	return &inst;
}

void rampool_imp::destroy()
{
	for (auto& pl : _pools)
		pl.destroy();
}

void* rampool_imp::malloc(size_t size)
{
	if (size == 0 || size > MAXSIZE)
		throw std::bad_alloc();

	auto idx = POOLINDEX(size);
	return _pools[idx].malloc(size);
}

void rampool_imp::free(void* p)
{
	if (!p)
		return;

	auto slt = POINTER_TO_SLOT(p);
	_pools[POOLINDEX(slt->normalize_size)].free(p);
}

void* rampool_imp::realloc(void* p, size_t size)
{
	if (!p)
		return malloc(size);

	auto slt = POINTER_TO_SLOT(p);
	assert(slt->owner == &_pools[POOLINDEX(slt->normalize_size)]);
	assert(slt->valid == valid_t::SLOT_USED);

	if (size <= slt->normalize_size)
	{
		_pools[POOLINDEX(slt->normalize_size)].total() += size - slt->actual_size;
		slt->actual_size = size;
		return p;
	}

	auto pm = malloc(size);
	memmove(pm, p, slt->actual_size);
	free(p);
	return pm;
}

void rampool_imp::leak(leak_info* info)
{
	if (!info)
		return;

	memset(info, 0, sizeof(leak_info));
	for (auto& pl : _pools)
	{
		info->count += pl.count();
		info->total_size += pl.count() * pl.get_size();
		info->total_actual_size += pl.total();
	}

	assert(info->total_actual_size <= info->total_size);
}

size_t rampool_imp::size(void* p)
{
	auto slt = POINTER_TO_SLOT(p);

	if (slt->valid != valid_t::SLOT_USED)
		return 0;

	return slt->actual_size;
}

void rampool_imp::gc()
{
	for (auto& pl : _pools)
		pl.gc();
}

void rampool_imp::auto_gc(bool b)
{
	_is_auto_gc = b;

	if (_is_auto_gc)
	{
		if (_auto_gc_thrd.joinable())
			return;

		_auto_gc_thrd = thread([this]()
			{
				while (_is_auto_gc)
				{
					for (auto& pl : _pools)
					{
						if (pl.need_gc())
							pl.gc();
					}

					this_thread::yield();
				}
			});
	}
	else
	{
		if (_auto_gc_thrd.joinable())
			_auto_gc_thrd.join();
	}
}