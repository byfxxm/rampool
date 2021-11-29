#pragma once

namespace rampool
{
	class spin
	{
	public:
		void lock()
		{
			while (flg.test_and_set(std::memory_order_relaxed))
				std::this_thread::yield();
		}

		void unlock()
		{
			flg.clear(std::memory_order_relaxed);
		}

	private:
		std::atomic_flag flg;
	};
}