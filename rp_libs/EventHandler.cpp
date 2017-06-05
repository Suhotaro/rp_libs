#include "stdafx.h"

#include "EventHandler.h"

EventHandler::EventHandler(HANDLE _event, std::function<bool(WSAEVENT)> _handler)
	: event(_event), handler(_handler)
{}

EventHandler::~EventHandler()
{
	if (event)
		WSACloseEvent(event);
}

EventHandler::EventHandler(const EventHandler &other)
{
	if (this == &other)
		return;

	event = other.event;
	handler = other.handler;
}

bool EventHandler::CanHandle(HANDLE _event)
{
	return event == _event;
}

bool EventHandler::Handle()
{
	bool ret = handler(event);
	WSAResetEvent(event);
	return ret;
}


const HANDLE EventHandler::Handler() const
{
	return event;
}