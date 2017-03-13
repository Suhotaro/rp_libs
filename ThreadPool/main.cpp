// ThreadPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <thread>
#include <chrono>
#include <memory>

#include "ThreadControl.h"
#include "ThreadPinger.h"

int main()
{
	std::shared_ptr<ThreadPool> threadPool = std::make_shared<ThreadPool>();
	std::shared_ptr<Pinger> pinger = std::make_shared<Pinger>("Pinger", threadPool);

	printf("Statrt\n");
	//threadPool->StartAll();
	pinger->Start();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Stop\n");
	//threadPool->StopAll();
	pinger->Stop();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Awake\n");
	//threadPool->AwakeAll();
	pinger->Awake();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Done\n");
	//threadPool->DoneAll();
	pinger->Done();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	printf("Exit\n");
    return 0;
}

