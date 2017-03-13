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
	
	if (__action == DtorAction::join)
		__t.join();
	else
		__t.detach();
}

std::thread& ThreadRAII::get() {
	return __t;
}