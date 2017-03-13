#pragma once

#include <memory>
#include <map>
#include "ThreadControl.h"
#include "ThreadRunner.h"

static const int MAX_THREADS = 10;

class ThreadControl;
class ThreadRunner;

class ThreadPool
{
public:
	ThreadPool(int max_threads = MAX_THREADS);
	~ThreadPool() {};

	std::weak_ptr<ThreadControl> createThread(std::shared_ptr<ThreadRunner> runner);	

	void StartAll();
	void StopAll();
	void AwakeAll();
	void DoneAll();

private:
	int __max_threads;
	std::map<std::thread::id, std::shared_ptr<ThreadControl> > __threads;

};