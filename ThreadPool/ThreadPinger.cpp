#include "stdafx.h"

#include <string>
#include <thread>
#include <chrono>

#include "ThreadControl.h"
#include "ThreadPinger.h"


void Pinger::Task()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("Ping\n");
}

