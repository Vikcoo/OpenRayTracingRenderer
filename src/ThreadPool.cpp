#include "ThreadPool.h"

void ThreadPool::WorkerThread(ThreadPool* master)
{
	while (master->m_alive) {
		//如果没有任务，则休眠
		if (master->m_tasks.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			continue;
		}
		//如果有任务，则执行任务
		Task* task = master->getTask();
		if (task != nullptr) {
			task->run();
			delete task;
			master->pending_task_count--;
		}else {
			std::this_thread::yield();
		}
	}
}

ThreadPool::ThreadPool(int numThreads)
{
	m_alive = true;
	pending_task_count = 0;
	if (numThreads == 0) {
		numThreads = std::thread::hardware_concurrency();
	}
	for (int i = 0; i < numThreads; i++) {
		m_threads.emplace_back(std::thread(ThreadPool::WorkerThread, this));
	}
}

ThreadPool::~ThreadPool()
{
	//等待所有任务完成
	wait();
	//线程池不活跃
	m_alive = 0;
	//等待所有线程退出
	for (auto& thread : m_threads) {
		thread.join();
	}
	//清空
	m_threads.clear();
}

class ParallelForTask : public Task {
public:
	ParallelForTask(size_t x, size_t y,size_t chunkWidth, size_t chunkHeight, const std::function<void(size_t, size_t)>& lambda)
		:x(x), y(y), chunkWidth(chunkWidth), chunkHeight(chunkHeight), lambda(lambda) {}

	void run() override {
		for (size_t i = 0; i < chunkHeight; i++) {
			for (size_t j = 0; j < chunkWidth; j++) {
				lambda(x + j, y + i);
			}
		}
		//lambda(x, y);
	}
private:
	size_t x, y;
	size_t chunkWidth, chunkHeight;
	std::function<void(size_t, size_t)> lambda;
};

void ThreadPool::parallelFor(size_t width, size_t height, const std::function<void(int, int)>& lambda)
{
	Guard guard(m_lock);

	float chuck_width_float = static_cast<float>(width) / sqrt(16) / sqrt(m_threads.size());
	float chuck_height_float = static_cast<float>(height) / sqrt(16) / sqrt(m_threads.size());

	size_t chuck_width = std::ceil(chuck_width_float);
	size_t chuck_height = std::ceil(chuck_height_float);

	for (size_t y = 0; y < height; y+=chuck_height) {
		for (size_t x = 0; x < width; x+=chuck_width) {
			//m_tasks.push_back(new ParallelForTask(x, y, lambda));
			if (x + chuck_width > width) {
				chuck_width = width - x;
			}
			if (y + chuck_height > height) {
				chuck_height = height - y;
			}
			addTask(new ParallelForTask(x, y,chuck_width, chuck_height, lambda));
		}
	}
}

void ThreadPool::wait() const
{
	//等待所有任务完成
	while (pending_task_count > 0) std::this_thread::yield();
}

void ThreadPool::addTask(Task* task)
{
	//Guard guard(m_lock);
	m_tasks.push(task);
	pending_task_count++;
}

Task* ThreadPool::getTask()
{
	Guard guard(m_lock);
	if (m_tasks.empty()) {
		return nullptr;
	}
	Task* task = m_tasks.front();
	m_tasks.pop();
	return task;
}
