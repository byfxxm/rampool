#pragma once

namespace rampool
{
	class spin_c
	{
	public:
		void lock()
		{
			while (flag.test_and_set(std::memory_order_relaxed))
				std::this_thread::yield();
		}

		void unlock()
		{
			flag.clear(std::memory_order_relaxed);
		}

	private:
		std::atomic_flag flag;
	};
}