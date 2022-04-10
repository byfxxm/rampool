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
		void leak(leak_info_s*);
		size_t size(void*);
		void gc();
		void auto_gc(bool);

	private:
		slot* __slot_cast(void*) const;

	private:
		std::array<pool_c, POOLNUM> __pools;
		std::thread __auto_gc_thread;
		volatile bool __is_auto_gc{ false };
	};
}