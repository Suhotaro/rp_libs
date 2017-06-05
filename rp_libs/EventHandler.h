#pragma once

#include <WinSock2.h>
#include "rp_libs.h"
#include <functional>

class EventHandler
{
public:
	EventHandler(HANDLE event, std::function<bool(WSAEVENT)> handler);
	~EventHandler();

	EventHandler(const EventHandler&);

	bool CanHandle(HANDLE event);
	bool Handle();

	const HANDLE Handler() const;
private:
	HANDLE event;
	std::function<bool(WSAEVENT)> handler;
};