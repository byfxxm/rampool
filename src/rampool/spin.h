#pragma once

class spin
{
public:
	void lock()
	{
		while (flg.test_and_set(memory_order_relaxed))
			this_thread::yield();
	}

	void unlock()
	{
		flg.clear();
	}

private:
	atomic_flag flg;
};