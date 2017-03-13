#pragma once

#include <memory>
#include "ThreadControl.h"
#include "ThreadPool.h"

class ThreadPool;
class ThreadControl;

class ThreadRunner : std::enable_shared_from_this<ThreadRunner>
{
public:
	ThreadRunner(std::string threadName, std::shared_ptr<ThreadPool> pool);
	~ThreadRunner() = default;

	/* TODO: forbid assignments and moving */

	void Start();
	void Done();
	void Awake();
	void Stop();

	virtual void Task() = 0;

	std::string getThreadName() const;

private:
	std::weak_ptr<ThreadControl> __threadControl;

	unsigned int ID;
	std::string __threadName;
	std::shared_ptr<ThreadPool> __pool;
};
