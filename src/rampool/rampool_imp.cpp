#include "stdafx.h"
#include "rampool_imp.h"
#include "Block.h"
#include "Slot.h"

RampoolImp::RampoolImp() {
	size_t size = 0;
	for_each(pools_.begin(), pools_.end(), [&](auto& it) {
		size += GRANULARITY;
		it.Initialize(size, this);
		});
}

RampoolImp::~RampoolImp() {
	AutoGc(false);
	Destroy();
}

RampoolImp* RampoolImp::Instance() {
	static RampoolImp inst;
	return &inst;
}

void RampoolImp::Destroy() {
	for (auto& pool : pools_)
		pool.Destroy();
}

void* RampoolImp::Malloc(size_t size) {
	if (size == 0 || size > MAXSIZE)
		return nullptr;

	return pools_[POOLINDEX(size)].Malloc(size);
}

void RampoolImp::Free(void* p) {
	if (!p)
		return;

	pools_[POOLINDEX(SlotCast(p)->normalize_size)].Free(p);
}

void* RampoolImp::Realloc(void* p, size_t size) {
	if (!p)
		return Malloc(size);

	auto slot = SlotCast(p);
	auto p1 = Malloc(size);
	memmove(p1, p, min(slot->actual_size, size));
	Free(p);
	return p1;
}

void RampoolImp::Leak(LeakInfo* info) {
	if (!info)
		return;

	memset(info, 0, sizeof(LeakInfo));
	for (auto& pool : pools_) {
		info->count += pool.Count();
		info->total_size += pool.Count() * pool.GetSize();
		info->total_actual_size += pool.Total();
	}

	assert(info->total_actual_size <= info->total_size);
}

size_t RampoolImp::Size(void* p) {
	return SlotCast(p)->actual_size;
}

void RampoolImp::Gc() {
	for (auto& pool : pools_)
		pool.Gc();
}

void RampoolImp::AutoGc(bool b) {
	is_auto_gc_ = b;

	if (is_auto_gc_) {
		if (auto_gc_thread_.joinable())
			return;

		auto_gc_thread_ = std::thread([this]() {
			while (is_auto_gc_) {
				for (auto& pool : pools_) {
					if (pool.NeedGc())
						pool.Gc();
				}

				std::this_thread::yield();
			}
			});
	} else {
		if (auto_gc_thread_.joinable())
			auto_gc_thread_.join();
	}
}

inline Slot* RampoolImp::SlotCast(void* p) const {
	auto slot = POINTER_TO_slot_s(p);

	if (slot->owner != this || slot->valid != Slot::Valid::kUsed)
		throw std::exception("invalid ptr");

	return slot;
}