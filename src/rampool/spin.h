#pragma once

namespace rampool {
	class Spin {
	public:
		void lock() {
			while (flag_.test_and_set(std::memory_order_relaxed))
				std::this_thread::yield();
		}

		void unlock() {
			flag_.clear(std::memory_order_relaxed);
		}

	private:
		std::atomic_flag flag_;
	};
}