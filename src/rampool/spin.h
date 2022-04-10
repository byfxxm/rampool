#pragma once

namespace rampool
{
	class spin
	{
	public:
		void lock()
		{
			while (__flag.test_and_set(std::memory_order_relaxed))
				std::this_thread::yield();
		}

		void unlock()
		{
			__flag.clear(std::memory_order_relaxed);
		}

	private:
		std::atomic_flag __flag;
	};
}