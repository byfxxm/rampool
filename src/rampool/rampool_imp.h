#pragma once
#include "rampool.h"
#include "pool.h"

namespace rampool {
	class RampoolImp {
	public:
		RampoolImp();
		~RampoolImp();
		static RampoolImp* Instance();
		void* Malloc(size_t);
		void Free(void*);
		void* Realloc(void*, size_t);
		void Destroy();
		void Leak(LeakInfo*);
		size_t Size(void*);
		void Gc();
		void AutoGc(bool);

	private:
		Slot* SlotCast(void*) const;

	private:
		std::array<Pool, POOLNUM> pools_;
		std::jthread auto_gc_thread_;
	};
}