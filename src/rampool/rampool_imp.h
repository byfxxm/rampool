#pragma once
#include "rampool.h"
#include "pool.h"

namespace rampool
{
	class rampool_imp_c
	{
	public:
		rampool_imp_c();
		~rampool_imp_c();
		static rampool_imp_c* instance();
		void* malloc(size_t);
		void free(void*);
		void* realloc(void*, size_t);
		void destroy();
		void leak(leak_info*);
		size_t size(void*);
		void gc();
		void auto_gc(bool);

	private:
		slot_s* __slot_s_cast(void*) const;

	private:
		std::array<pool, POOLNUM> __pools;
		std::thread __auto_gc_thrd;
		volatile bool __is_auto_gc{ false };
	};
}