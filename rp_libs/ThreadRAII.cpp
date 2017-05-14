#include "stdafx.h"

#include <thread>
#include <string>

#include "ThreadRAII.h"

ThreadRAII::ThreadRAII(std::thread&& t, DtorAction action)
	: __t(std::move(t)), __action(action)
{}

ThreadRAII::~ThreadRAII() {
	if (__t.joinable() == false)
		return;
	
	/* TODO: should add features like wait untill a thread is closed for certain time and
	force to destoy it if the thread is locked */
	if (__action == DtorAction::join)
		__t.join();
	else
		__t.detach();
}

std::thread& ThreadRAII::get() {
	return __t;
}