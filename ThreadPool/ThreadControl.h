#pragma once

#include <string>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>

#include "ThreadPool.h"
#include "ThreadRAII.h"
#include "ThreadRunner.h"

class ThreadPool;

class ThreadControl
{
public:
	explicit ThreadControl(ThreadRunner *runner);

	void Start();
	void Done();
	void Awake();
	void Stop();

	void Task();	

	unsigned int ThreadControl::getID() const;
	std::string getRelatedThreadName() const;

private:
	void Init();
	void WaitUntillStoped();

private:
	enum class State {
		NONE = 0,
		RUN,
		STOPED,
		DONE,
	};
	
	static unsigned int __InnerCounterOfIDs;
	unsigned int __threadID;
	ThreadRunner* __runner;

	std::unique_ptr<ThreadRAII> __threadHolder;
	std::once_flag __startFlag;
	
	State __state;
	std::mutex __stateMutex;
	std::condition_variable __stateCondVariable;
};