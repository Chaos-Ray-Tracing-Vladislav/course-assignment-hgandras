#pragma once

#include<mutex>
#include<thread>
#include<vector>
#include<queue>
#include<functional>

struct ThreadPool
{
private:
	std::vector<std::jthread> threads;
	std::mutex taskQueueMutex;
	std::condition_variable mutexCondition;
	std::condition_variable allFinishedCondition;
	std::queue<std::function<void()>> taskQueue;
	bool stop = false;
	void Worker();
	int busyThreads = 0;

public:
	void WaitForAll();
	void Start(const int);
	void AddTask(const std::function<void()>&);
	void Stop();
	bool Busy();
	int queueSize();
	~ThreadPool();
};