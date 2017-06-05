#include "stdafx.h"
#include "Util.h"
#include "NetUtil.h"
#include "SelectorClient.h"
#include "CyclicBuffer.h"
#include "SocketTCP.h"

#include <functional>
#include <stdio.h>

#define BUFF_SIZE 4096

rpSelectorClient::rpSelectorClient(std::string address, int port, CyclicBuffer *inBuf, CyclicBuffer *outBuf, int flags)
: eInherited(),
__address(address),
__port(port),
__inBuf(inBuf),
__outBuf(outBuf),
__flags(flags),
__size(BUFF_SIZE)
{}

rpSelectorClient::~rpSelectorClient()
{}

bool rpSelectorClient::Init()
{
	__socket = new rpTCPClientSocket(__address, __port);
	RETURNVALIFFALSE(__socket, false, "allocate __socket");

	int ret = __socket->Init();
	RETURNVALIFFALSE(ret == true, false, "Selector Init");

	__buffer = new char[__size];
	RETURNVALIFFALSE(__buffer, false, "allocate size for buffer");

	std::shared_ptr<EventHandler> signalDoneHandler(
		new EventHandler(WSACreateEvent(), std::bind(&rpSelectorClient::processEventDone, this, std::placeholders::_1)));
	RETURNVALIFFALSE(signalDoneHandler->Handler(), false, "WSACreateEvent failed");
	
	std::shared_ptr<EventHandler> signalSocketHandler(
		new EventHandler(WSACreateEvent(), std::bind(&rpSelectorClient::processEventSocket, this, std::placeholders::_1)));
	RETURNVALIFFALSE(signalSocketHandler->Handler(), false, "WSACreateEvent failed");

	ret = WSAEventSelect(__socket->Handle(), signalSocketHandler->Handler(), __flags);
	NETRETVALIFFALSE(ret != (int)SOCKET_ERROR, false, "WSAEventSelect failed");

	std::shared_ptr<EventHandler> signalWriteHandler(
		new EventHandler(__outBuf->Signal(), std::bind(&rpSelectorClient::processEventDataToWrite, this, std::placeholders::_1)));

	events.push_back(signalDoneHandler->Handler());
	eventHandlers.push_back(std::move(signalDoneHandler));

	events.push_back(signalSocketHandler->Handler());
	eventHandlers.push_back(std::move(signalSocketHandler));

	events.push_back(signalWriteHandler->Handler());
	eventHandlers.push_back(std::move(signalWriteHandler));

	return true;
}

void rpSelectorClient::Done()
{
	//TODO: weired
	WSASetEvent(events[0]);
}

bool rpSelectorClient::Update()
{
	DWORD Index;
	bool ret = false;

	printf("Client: Update\n");

	//while (true)
	//{
		Index = WSAWaitForMultipleEvents(events.size(),
										 &events[0],
										 false,
										 WSA_INFINITE,
										 false);

		WSAEVENT event = events[Index - WSA_WAIT_EVENT_0];
		for (int i = 0; i < eventHandlers.size(); i++)
		{
			if (eventHandlers[i]->CanHandle(event))
			{
				eventHandlers[i]->Handle();
				ret = true;
				break;
			}
		}
	//}

	return ret;
}

int rpSelectorClient::Errors()
{
	return 0;
}

bool rpSelectorClient::processEventSocket(WSAEVENT event)
{
	WSANETWORKEVENTS NetworkEvents;
	int data_read;

	WSAEnumNetworkEvents(__socket->Handle(), event, &NetworkEvents);
	if (NetworkEvents.lNetworkEvents && FD_READ)
	{
		if (NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
		{
			printf("FD_READ: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_READ_BIT]);
			return false;
		}

		data_read = recv(__socket->Handle(), __buffer, __size, 0);
		if (data_read == 0)
			OnClose();
		else
			OnRead(__buffer, data_read);
	}
	else if (NetworkEvents.lNetworkEvents && FD_CLOSE)
	{
		if (NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			printf("FD_CLOSE: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
			return false;
		}

		closesocket(__socket->Handle());
	}
	else if (NetworkEvents.lNetworkEvents && FD_WRITE
		|| NetworkEvents.lNetworkEvents && FD_OOB
		|| NetworkEvents.lNetworkEvents && FD_ACCEPT
		|| NetworkEvents.lNetworkEvents && FD_CONNECT
		|| NetworkEvents.lNetworkEvents && FD_QOS
		|| NetworkEvents.lNetworkEvents && FD_GROUP_QOS
		|| NetworkEvents.lNetworkEvents && FD_ROUTING_INTERFACE_CHANGE
		|| NetworkEvents.lNetworkEvents && FD_ADDRESS_LIST_CHANGE
		|| NetworkEvents.lNetworkEvents && FD_CONNECT)
	{
		//TODO: implement if required
	}
	return true;
}

bool rpSelectorClient::processEventDone(WSAEVENT event)
{
	closesocket(__socket->Handle());
	return true;
}

bool rpSelectorClient::processEventDataToWrite(WSAEVENT event)
{
	int ret;
	int data_write;

	OnWrite(__buffer, &data_write);
	//write until there are data left to send
	ret = send(__socket->Handle(), __buffer, data_write, 0);
	//if (ret == 0)
	//	OnClose();

	return true;
}

void rpSelectorClient::OnRead(char *buffer, int size)
{
	__inBuf->Add(buffer, size);
}

void rpSelectorClient::OnWrite(char *buffer, int *size)
{
	__outBuf->Pop(buffer, size, __size);
}

void rpSelectorClient::OnClose()
{

}