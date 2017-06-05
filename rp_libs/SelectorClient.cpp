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
	RETURNVALIFFALSE(__socket->Init() == true, false, "Selector Init");
	InitNetworkEvents();
	RETURNVALIFFALSE(InitEvents() == true, false, "allocate size for buffer");
	__buffer = new char[__size];
	RETURNVALIFFALSE(__buffer, false, "allocate size for buffer");

	return true;
}

bool rpSelectorClient::InitEvents()
{
	std::shared_ptr<EventHandler> signalDoneHandler(
		new EventHandler(WSACreateEvent(), std::bind(&rpSelectorClient::processEventDone, this, std::placeholders::_1)));
	RETURNVALIFFALSE(signalDoneHandler->Handler(), false, "WSACreateEvent failed");

	std::shared_ptr<EventHandler> signalSocketHandler(
		new EventHandler(WSACreateEvent(), std::bind(&rpSelectorClient::processEventSocket, this, std::placeholders::_1)));
	RETURNVALIFFALSE(signalSocketHandler->Handler(), false, "WSACreateEvent failed");

	int ret = WSAEventSelect(__socket->Handle(), signalSocketHandler->Handler(), __flags);
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

void rpSelectorClient::InitNetworkEvents()
{
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_READ, FD_READ_BIT, [&]() {
		int data_read;
		data_read = recv(__socket->Handle(), __buffer, __size, 0);
		if (data_read == 0)
			OnClose();
		else
			OnRead(__buffer, data_read);
	}));

	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_CLOSE, FD_CLOSE_BIT, [&]() {
		closesocket(__socket->Handle());
	}));

#if 0
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_WRITE, FD_WRITE_BIT, [&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_OOB, FD_OOB_BIT, [&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_ACCEPT, FD_ACCEPT_BIT, [&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_CONNECT, FD_CONNECT_BIT, [&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_QOS, FD_QOS_BIT, [&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_GROUP_QOS, FD_GROUP_QOS_BIT, [&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_ROUTING_INTERFACE_CHANGE, FD_ROUTING_INTERFACE_CHANGE_BIT,
		[&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_ADDRESS_LIST_CHANGE, FD_ADDRESS_LIST_CHANGE_BIT, [&]() {}));
	eventHandlersNet.push_back(std::make_shared<NetworkEventHandler>(
		FD_CONNECT, FD_CONNECT_BIT, [&]() {}));
#endif
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
		Index = WSAWaitForMultipleEvents(events.size(), &events[0], false, WSA_INFINITE, false);
		WSAEVENT event = events[Index - WSA_WAIT_EVENT_0];
		for (int i = 0; i < eventHandlers.size(); i++)
			if (eventHandlers[i]->CanHandle(event))
				return eventHandlers[i]->Handle();
	//}

	return false;
}

int rpSelectorClient::Errors()
{
	return 0;
}

bool rpSelectorClient::processEventSocket(WSAEVENT event)
{
	WSANETWORKEVENTS NetworkEvents;
	WSAEnumNetworkEvents(__socket->Handle(), event, &NetworkEvents);
	for (int i = 0; i < eventHandlersNet.size(); i++)
		if (eventHandlersNet[i]->CanHandle(&NetworkEvents))
		{
			eventHandlersNet[i]->Handle();
			break;
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
	int dataWrite;
	OnWrite(__buffer, &dataWrite);

	int ret = 0;
	int idx = 0;
	int dataLeft = dataWrite;

	while (dataLeft > 0)
	{
		ret = send(__socket->Handle(), __buffer, dataLeft, 0);
		if (ret == 0)
			OnClose();
		else if (ret == SOCKET_ERROR)
		{
			printf("ERROR: failed: %d\n", WSAGetLastError());
			return false;
		}

		dataLeft -= ret;
		idx += ret;
	}

	return true;
}

void rpSelectorClient::OnRead(char *buffer, int size)
{
	buffer[size - 1] = '\0';
	printf("OnRead: %s\n", buffer);
	__inBuf->Add(buffer, size);
}

void rpSelectorClient::OnWrite(char *buffer, int *size)
{
	__outBuf->Pop(buffer, size, __size);
}

void rpSelectorClient::OnClose()
{
	printf("Close\n");
}