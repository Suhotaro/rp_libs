// ThreadPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <thread>
#include <chrono>
#include <memory>

#include "ThreadControl.h"
#include "ThreadPinger.h"

void testPinger() {
	std::shared_ptr<ThreadPool> threadPool = std::make_shared<ThreadPool>();
	std::shared_ptr<Pinger> pinger = std::make_shared<Pinger>("Pinger", threadPool);

	printf("Pinger: Statrt\n");
	pinger->Start();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Pinger: Stop\n");
	pinger->Stop();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Pinger: Awake\n");
	pinger->Awake();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Pinger: Done\n");
	pinger->Done();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	threadPool->showAllIDs();
	threadPool->showAllNames();

	printf("Pinger: Exit\n");
}

void testPool() {
	std::shared_ptr<ThreadPool> threadPool = std::make_shared<ThreadPool>();
	std::shared_ptr<Pinger> pinger = std::make_shared<Pinger>("Pinger1", threadPool);
	std::shared_ptr<Pinger> pinger2 = std::make_shared<Pinger>("Pinger2", threadPool);

	printf("Pool: Statrt\n");
	threadPool->StartAll();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Pool: Stop\n");
	threadPool->StopAll();	
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Pool: Awake\n");
	threadPool->AwakeAll();	
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Pool: Done\n");
	threadPool->DoneAll();	
	std::this_thread::sleep_for(std::chrono::seconds(3));

	threadPool->showAllIDs();
	threadPool->showAllNames();

	printf("Pool: runner2 ID:%d\n", pinger2->getThreadID());

	printf("Pool: Exit\n");
}

int main()
{
	//testPinger();
	testPool();

    return 0;
}

