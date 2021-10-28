#pragma once

class spin
{
public:
	void lock()
	{
		while (flag.test_and_set(memory_order_relaxed))
			this_thread::yield();
	}

	void unlock()
	{
		flag.clear(memory_order_relaxed);
	}

private:
	atomic_flag flag;
};