#include "stdafx.h"
#include "Util.h"
#include "NetUtil.h"
#include "SelectorClient.h"
#include "CyclicBuffer.h"
#include "SocketTCP.h"

#include <stdio.h>

#define BUFF_SIZE 4096

rpSelectorClient::rpSelectorClient(CyclicBuffer *inBuf, CyclicBuffer *outBuf, int flags)
: eInherited(),
__inBuf(inBuf),
__outBuf(outBuf),
__flags(flags),
__size(BUFF_SIZE)
{}

rpSelectorClient::~rpSelectorClient()
{
	WSACloseEvent(__signalDone);
	WSACloseEvent(__signalSocket);
}

bool rpSelectorClient::Init()
{
	int ret = __socket->Init();
	if (ret)
	{
		printf("ERROR: Selector Init\n");
		return false;
	}

	__buffer = new char[__size];
	RETURNVALIFFALSE(__buffer, false, "allocate size for buffer");

	__signalDone = WSACreateEvent();
	RETURNVALIFFALSE(__signalDone, false, "WSACreateEvent failed");

	__signalSocket = WSACreateEvent();
	RETURNVALIFFALSE(__signalSocket, false, "WSACreateEvent failed");

	ret = WSAEventSelect(__socket->Handle(), __signalSocket, __flags);
	NETRETVALIFFALSE(ret != SOCKET_ERROR, "WSAEventSelect failed");

	events.push_back(__signalSocket);
	events.push_back(__signalDone);
	events.push_back(__outBuf->Signal());

	return true;
}

void rpSelectorClient::Done()
{
	WSASetEvent(__signalDone);
}

bool rpSelectorClient::Update()
{
	DWORD Index;


	while (true)
	{
		Index = WSAWaitForMultipleEvents(events.size(),
										 &events[0],
										 false,
										 WSA_INFINITE,
										 false);

		WSAEVENT event = events[Index - WSA_WAIT_EVENT_0];
		if (event == __signalSocket)
		{
			if (false == processEventSocket(event))
			{
				printf("ERROR: process SIGNAL signal\n");
				return false;
			}
		}
		else if (event == __signalDone)
		{
			if (false == processEventDone(event))
			{
				printf("ERROR: process DONE signal\n");
				return false;
			}
		}
		else if (event == __outBuf->Signal())
		{
			if (false == processEventDataToWrite(event))
			{
				printf("ERROR: process DATA_TO_WRITE signal\n");
				return false;
			}
		}
		else
		{
			printf("ERROR: unknown event\n");
			return false;
		}
	}

	return true;
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
	else if (NetworkEvents.lNetworkEvents && FD_WRITE)
	{
		if (NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
		{
			printf("FD_WRITE: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_WRITE_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_OOB)
	{
		if (NetworkEvents.iErrorCode[FD_OOB_BIT] != 0)
		{
			printf("FD_OOB: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_OOB_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_ACCEPT)
	{
		if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
		{
			printf("FD_ACCEPT: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_CONNECT)
	{
		if (NetworkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
		{
			printf("FD_CONNECT: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_CONNECT_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_QOS)
	{
		if (NetworkEvents.iErrorCode[FD_QOS_BIT] != 0)
		{
			printf("FD_CONNECT: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_QOS_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_GROUP_QOS)
	{
		if (NetworkEvents.iErrorCode[FD_GROUP_QOS_BIT] != 0)
		{
			printf("FD_CONNECT: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_GROUP_QOS_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_ROUTING_INTERFACE_CHANGE)
	{
		if (NetworkEvents.iErrorCode[FD_ROUTING_INTERFACE_CHANGE_BIT] != 0)
		{
			printf("FD_CONNECT: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_ROUTING_INTERFACE_CHANGE_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_ADDRESS_LIST_CHANGE)
	{
		if (NetworkEvents.iErrorCode[FD_ADDRESS_LIST_CHANGE_BIT] != 0)
		{
			printf("FD_CONNECT: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_ADDRESS_LIST_CHANGE_BIT]);
			return false;
		}

		//TODO: is required
	}
	else if (NetworkEvents.lNetworkEvents && FD_CONNECT)
	{
		if (NetworkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
		{
			printf("FD_CONNECT: failed with error: %d\n",
				NetworkEvents.iErrorCode[FD_CONNECT_BIT]);
			return false;
		}

		//TODO: is required
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

void rpSelectorClient::OnRead(char *__buffer, int __size)
{

}

void rpSelectorClient::OnWrite(char *__buffer, int *__size)
{

}

void rpSelectorClient::OnClose()
{

}