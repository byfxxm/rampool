#pragma once
#include "stack.h"
#include "block.h"
#include "slot.h"
#include "spin.h"

namespace rampool {
	class Pool {
	private:
		using Mutex = Spin;
		using Lock = std::lock_guard<Mutex>;

	public:
		void Initialize(size_t, const void*);
		size_t GetSize();
		void* Malloc(size_t);
		void Free(void*);
		void Destroy();
		size_t Count();
		size_t Total();
		void Gc();
		bool NeedGc();

	private:
		Mutex mutex_;
		Stack<Block> block_stack_;
		Stack<Slot> free_stack_;
		size_t size_{ 0 };
		size_t count_{ 0 };
		size_t total_{ 0 };
		const void* owner_{ nullptr };
	};
}