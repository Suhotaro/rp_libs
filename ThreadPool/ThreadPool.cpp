#include "stdafx.h"

#include "ThreadPool.h"
#include "ThreadControl.h"
#include "ThreadRunner.h"

ThreadPool::ThreadPool(int max_threads)
	: __max_threads(max_threads)
{}

std::weak_ptr<ThreadControl> ThreadPool::createThread(std::shared_ptr<ThreadRunner> runner)
{
	std::shared_ptr<ThreadControl> threadControl = std::make_shared<ThreadControl>(runner);
	threadControl->Start();
	std::thread::id threadID = threadControl->getID();
	__threads.insert(std::pair<std::thread::id, std::shared_ptr<ThreadControl> >(threadID, threadControl));

	return threadControl;
}

void ThreadPool::StartAll()
{
	if (__threads.empty())
		return;

	for (auto pair : __threads)
	{
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Start();
	}
}

void ThreadPool::StopAll()
{
	if (__threads.empty())
		return;

	for (auto pair : __threads)
	{
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Stop();
	}
}

void ThreadPool::AwakeAll()
{
	if (__threads.empty())
		return;

	for (auto pair : __threads)
	{
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Awake();
	}
}

void ThreadPool::DoneAll()
{
	if (__threads.empty())
		return;

	for (auto pair : __threads)
	{
		std::shared_ptr<ThreadControl> thread = pair.second;
		/* TODO: check if thread is valid */
		thread->Done();
	}
}