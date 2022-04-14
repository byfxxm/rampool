#include "stdafx.h"
#include "rampool_imp.h"
#include "Block.h"
#include "Slot.h"

RampoolImp::RampoolImp() {
	size_t Size = 0;
	for_each(pools_.begin(), pools_.end(), [&](auto& it) {
		Size += GRANULARITY;
		it.Initialize(Size, this);
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
	for (auto& po : pools_)
		po.Destroy();
}

void* RampoolImp::Malloc(size_t Size) {
	if (Size == 0 || Size > MAXSIZE)
		return nullptr;

	return pools_[POOLINDEX(Size)].Malloc(Size);
}

void RampoolImp::Free(void* p) {
	if (!p)
		return;

	pools_[POOLINDEX(SlotCast(p)->normalize_size)].Free(p);
}

void* RampoolImp::Realloc(void* p, size_t Size) {
	if (!p)
		return Malloc(Size);

	auto slt = SlotCast(p);
	auto p_ = Malloc(Size);
	memmove(p_, p, min(slt->actual_size, Size));
	Free(p);
	return p_;
}

void RampoolImp::Leak(leak_info_s* info) {
	if (!info)
		return;

	memset(info, 0, sizeof(leak_info_s));
	for (auto& po : pools_) {
		info->Count += po.Count();
		info->total_size += po.Count() * po.GetSize();
		info->total_actual_size += po.Total();
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
				for (auto& po : pools_)
				{
					if (po.NeedGc())
						po.Gc();
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
	auto slt = POINTER_TO_slot_s(p);

	if (slt->owner != this || slt->valid != Slot::Valid::USED)
		throw std::exception("invalid ptr");

	return slt;
}