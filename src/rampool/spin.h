#pragma once

class spin
{
public:
	void lock()
	{
		while (flg.test_and_set(memory_order_relaxed))
			yield();
	}

	void unlock()
	{
		flg.clear(memory_order_relaxed);
	}

private:
	atomic_flag flg;
};