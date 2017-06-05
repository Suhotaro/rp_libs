#pragma once

#include <WinSock2.h>
#include <functional>

class NetworkEventHandler
{
public:
	NetworkEventHandler(int flag, int bit, std::function<void()>);
	~NetworkEventHandler() {}

	bool CanHandle(WSANETWORKEVENTS *NetworkEvents);
	bool Handle();
private:
	int flag;
	int bit;
	std::function<void()> handle;
};