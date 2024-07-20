#include "Threadpool.h"

void ThreadPool::Start(int num_threads = std::jthread::hardware_concurrency())
{
	for (int i = 0; i < num_threads; i++)
		threads.emplace_back(std::jthread(&ThreadPool::Worker, this));
}

void ThreadPool::AddTask(const std::function<void()>& task)
{
	std::unique_lock<std::mutex> lock(taskQueueMutex);
	taskQueue.emplace(task);
	lock.unlock();
	mutexCondition.notify_one();
}

void ThreadPool::Stop() {
	std::unique_lock<std::mutex> lock(taskQueueMutex);
	stop = true;
	lock.unlock();
	mutexCondition.notify_all();
	threads.clear(); //Since jthread is used this is easier.
}

void ThreadPool::Worker()
{
	std::unique_lock<std::mutex> lock(taskQueueMutex);
	std::function<void()> task;
	while (true) {
		mutexCondition.wait(lock, [this] {
			return stop || !taskQueue.empty();
			});
		if (stop)
			return;
		busyThreads++;
		task = std::move(taskQueue.front());
		taskQueue.pop();
		lock.unlock();
		task();
		lock.lock();
		--busyThreads;
		allFinishedCondition.notify_one();
	}
}

bool ThreadPool::Busy()
{
	std::unique_lock<std::mutex> lock(taskQueueMutex);
	bool busy = !taskQueue.empty() || busyThreads > 0;
	lock.unlock();
	return busy;
}

int ThreadPool::queueSize()
{
	std::unique_lock<std::mutex> lock(taskQueueMutex);
	int result = taskQueue.size();
	lock.unlock();
	return result;
}

ThreadPool::~ThreadPool() {
	Stop();
}

void ThreadPool::WaitForAll()
{
	std::unique_lock<std::mutex> lock(taskQueueMutex);
	allFinishedCondition.wait(lock, [this]() { return taskQueue.empty() && (busyThreads == 0); });
}