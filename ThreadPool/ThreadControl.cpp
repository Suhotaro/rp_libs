#include "stdafx.h"

#include <string>
#include <assert.h>
#include <thread>
#include <atomic>
#include <mutex>

#include "ThreadControl.h"

unsigned int ThreadControl::__InnerCounterOfIDs = 0;

ThreadControl::ThreadControl(ThreadRunner *runner)
	: __state(State::NONE), __runner(runner) {
	__threadID = __InnerCounterOfIDs++;
}

void ThreadControl::Start() {
	std::call_once(__startFlag, &ThreadControl::Init, this);
}

void ThreadControl::Init() {
	std::thread executor(&ThreadControl::Task, this);
	__state = State::RUN;
	__threadHolder = std::make_unique<ThreadRAII>(std::move(executor), ThreadRAII::DtorAction::join);
}

void ThreadControl::Done() {
	std::lock_guard<std::mutex> guard(__stateMutex);
	__state = State::DONE;
	__stateCondVariable.notify_one();
}

void ThreadControl::Awake() {
	std::lock_guard<std::mutex> guard(__stateMutex);
	if (__state == State::RUN)
		return;

	__state = State::RUN;
	__stateCondVariable.notify_one();
}

void ThreadControl::Stop() {
	std::lock_guard<std::mutex> guard(__stateMutex);
	if (__state == State::STOPED)
		return;

	__state = State::STOPED;
}

void ThreadControl::Task() {
	/* TODO: should it be locked or allow it to make additional 
	loop before it is stoped */
	while (__state != State::DONE)
	{
		WaitUntillStoped();
		__runner->Task();
	}
}

void ThreadControl::WaitUntillStoped() {
	std::unique_lock<std::mutex> stateUniqueLock(__stateMutex);
	__stateCondVariable.wait(stateUniqueLock, [this]()
	{
		return __state != State::STOPED;
	});
}

unsigned int ThreadControl::getID() const {
	return __threadID;
}

std::string ThreadControl::getRelatedThreadName() const {
	return __runner->getThreadName();
}