#include "stdafx.h"
#include "NetworkEventHandler.h"

NetworkEventHandler::NetworkEventHandler(int _flag, int _bit, std::function<void()> _handle)
	: flag(_flag), bit(_bit), handle(_handle)
{}

bool NetworkEventHandler::CanHandle(WSANETWORKEVENTS *NetworkEvents)
{
	if (NetworkEvents->lNetworkEvents && flag)
	{
		if (NetworkEvents->iErrorCode[bit] != 0)
		{
			printf("ERROR: failed with error: %d\n", NetworkEvents->iErrorCode[bit]);
			return false;
		}

		return true;
	}

	return false;
}

bool NetworkEventHandler::Handle()
{
	handle();
	return true;
}