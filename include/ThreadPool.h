#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include "SpinLock.h"
class Task {
public:
    virtual void run() = 0;
    virtual ~Task() = default;
};


class ThreadPool {
public:
    static void WorkerThread(ThreadPool* pool);

    ThreadPool(int numThreads);
    ~ThreadPool();

    void parallelFor(size_t width, size_t height, const std::function<void(int, int)> &lambda);
    void wait() const;

    void addTask(Task* task);
    Task* getTask();
private:
    std::vector<std::thread> m_threads;
    std::queue<Task*> m_tasks;

    SpinLock m_lock;
    std::atomic<int> pending_task_count;
    std::atomic<int> m_alive;
};