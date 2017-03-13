#include "stdafx.h"

#include <memory>

#include "ThreadRunner.h"

ThreadRunner::ThreadRunner(std::string threadName, std::shared_ptr<ThreadPool> pool)
	: __threadName(threadName), __pool(pool) {
	if (__pool) {
		__threadControl = __pool->createThread(this);
	} else {
		__threadControl = std::make_shared<ThreadControl>(this);
	}
}

void ThreadRunner::Start() {
	if (auto controller = __threadControl.lock())
		controller->Start();
	else
		printf("Warining: ThreadRunner::Start: pointer has expired\n");
}

void ThreadRunner::Done() {
	if (auto controller = __threadControl.lock())
		controller->Done();
	else
		printf("Warining: ThreadRunner::Start: pointer has expired\n");
}

void ThreadRunner::Awake() {
	if (auto controller = __threadControl.lock())
		controller->Awake();
	else
		printf("Warining: ThreadRunner::Start: pointer has expired\n");
}

void ThreadRunner::Stop() {
	if (auto controller = __threadControl.lock())
		controller->Stop();
	else
		printf("Warining: ThreadRunner::Start: pointer has expired\n");
}

std::string ThreadRunner::getThreadName() const
{
	return __threadName;
}