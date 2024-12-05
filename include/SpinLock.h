#pragma once

#include <atomic>
#include <thread>

class SpinLock {
public:
	void acquire() { while (flag.test_and_set(std::memory_order_acquire)); }
	void release() { flag.clear(std::memory_order_release); }
private:
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

class Guard {
public:
	Guard(SpinLock& lock) : lock(lock) { lock.acquire(); }
	~Guard() { lock.release(); }
private:
	SpinLock& lock;
};