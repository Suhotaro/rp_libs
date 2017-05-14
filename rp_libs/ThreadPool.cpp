#include "stdafx.h"

#include "ThreadPool.h"
#include "ThreadControl.h"
#include "ThreadRunner.h"

ThreadPool::ThreadPool(int max_threads)
	: __max_threads(max_threads)
{}

std::weak_ptr<ThreadControl> ThreadPool::createThread(ThreadRunner* runner) {
	std::shared_ptr<ThreadControl> threadControl = std::make_shared<ThreadControl>(runner);
	int threadID = threadControl->getID();
	__threads.insert(std::pair<int, std::shared_ptr<ThreadControl> >(threadID, threadControl));

	return threadControl;
}

ThreadPool::~ThreadPool() {  
	if (!__threads.empty()) {
		for (auto pair : __threads) {
			std::shared_ptr<ThreadControl> thread = pair.second;
			thread->Done();
		}
	}
}

void ThreadPool::StartAll() {
	if (__threads.empty())
		return;

	for (auto pair : __threads) {
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Start();
	}
}

void ThreadPool::StopAll() {
	if (__threads.empty())
		return;

	for (auto pair : __threads) {
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Stop();
	}
}

void ThreadPool::AwakeAll() {
	if (__threads.empty())
		return;

	for (auto pair : __threads) {
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Awake();
	}
}

void ThreadPool::DoneAll() {
	if (__threads.empty())
		return;

	for (auto pair : __threads) {
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Done();
	}
}

void ThreadPool::showAllIDs() const {
	if (__threads.empty())
		return;

	for (auto pair : __threads) {
		std::shared_ptr<ThreadControl> thread = pair.second;
		printf("Thread ID: %ld\n", thread->getID());
	}
}

void ThreadPool::showAllNames() const
{
	if (__threads.empty())
		return;

	for (auto pair : __threads) {
		std::shared_ptr<ThreadControl> thread = pair.second;
		printf("Thread name: %s\n", thread->getRelatedThreadName().c_str());
	}
}

ThreadRunner* ThreadPool::getRunnerByName(std::string& name)
{
	if (!__threads.empty()) {
		for (auto pair : __threads) {
			std::shared_ptr<ThreadControl> thread = pair.second;
			if (!name.compare(thread->getRelatedThreadName()))
				return thread->getRunner();
		}
	}

	return nullptr;
}

ThreadRunner* ThreadPool::getRunnerByID(unsigned int key)
{
	using Pool = std::map<int, std::shared_ptr<ThreadControl> >;

	if (!__threads.empty()) {
		Pool::iterator it = __threads.find(key);
		if (it != __threads.end()) {
			std::shared_ptr<ThreadControl> thread = it->second;
			return thread->getRunner();
		}
	}

	return nullptr;
}