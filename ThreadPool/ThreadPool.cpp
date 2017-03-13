#include "stdafx.h"

#include "ThreadPool.h"
#include "ThreadControl.h"
#include "ThreadRunner.h"

ThreadPool::ThreadPool(int max_threads)
	: __max_threads(max_threads)
{}

std::weak_ptr<ThreadControl> ThreadPool::createThread(ThreadRunner* runner) {
	std::shared_ptr<ThreadControl> threadControl = std::make_shared<ThreadControl>(runner);
	unsigned int threadID = threadControl->getID();
	__threads.insert(std::pair<unsigned int, std::shared_ptr<ThreadControl> >(threadID, threadControl));

	return threadControl;
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