#include "stdafx.h"
#include "rampool_mt.h"
#include "rampool_imp.h"

rampool_mt::rampool_mt()
{
	gc_thd = thread([this]()
		{
			while (!is_exitting)
			{
				unique_lock<mutex> lck(mtx);
				cv.wait(lck);
				__gc();
			}
		});
}

rampool_mt::~rampool_mt()
{
	is_exitting = true;
	gc_thd.join();
}

rampool_mt& rampool_mt::instance()
{
	static rampool_mt inst;
	return inst;
}

void* rampool_mt::malloc(size_t size)
{
	return rampool_imp::instance_thread().malloc(size);
}

void rampool_mt::free(void* p)
{
	unique_lock<mutex> lck(mtx);
	free_deferred_list.push_back(p);

	if (free_deferred_list.size() > AUTOGC_THRESHOLD)
		gc();
}

void* rampool_mt::realloc(void*, size_t)
{
	return 0;
}

void rampool_mt::leak(leak_info*)
{

}

size_t rampool_mt::size(void*)
{
	return 0;
}

void rampool_mt::gc()
{
	cv.notify_all();
}

void rampool_mt::__gc()
{
	for (auto p : free_deferred_list)
	{
		auto slt = POINTER_TO_SLOT(p);
		((rampool_imp*)slt->owner)->free(p);
	}
}