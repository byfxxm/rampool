#pragma once
#include "stack.h"
#include "Block.h"
#include "Slot.h"
#include "Spin.h"

namespace rampool {
	class pool {
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
		Stack<Block> block_Stack_;
		Stack<Slot> free_Stack_;
		size_t size_{ 0 };
		size_t count_{ 0 };
		size_t total_{ 0 };
		const void* owner_{ nullptr };
	};
}