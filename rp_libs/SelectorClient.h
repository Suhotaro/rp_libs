#pragma once

#include "Socket.h"
#include "Selector.h"
#include "EventHandler.h"
#include "NetworkEventHandler.h"

#include <vector>
#include <string>
#include <memory>

class CyclicBuffer;

class RP_LIBS_API rpSelectorClient : public rpSelector
{
	typedef rpSelector eInherited;
public:
	rpSelectorClient(std::string address, int port, CyclicBuffer *inBuf, CyclicBuffer *outBuf, int flags);
	~rpSelectorClient();

	virtual bool Init() override;
	virtual void Done() override;
	virtual bool Update() override;
	virtual int Errors() override;



protected:
	void InitNetworkEvents();
	bool InitEvents();

	//Correct return value has to be returned to left Update properly
	bool processEventSocket(WSAEVENT event);
	bool processEventDone(WSAEVENT event);
	bool processEventDataToWrite(WSAEVENT event);

	virtual void OnRead(char *buffer, int size) override;
	virtual void OnWrite(char *buffer, int *size) override;
	virtual void OnClose() override;
	
	std::string __address;
	int __port;
	rpSocket *__socket;

	std::vector<SOCKET> sockets;
	std::vector<WSAEVENT> events;
	std::vector<std::shared_ptr<EventHandler>> eventHandlers;
	std::vector<std::shared_ptr<NetworkEventHandler>> eventHandlersNet;

	CyclicBuffer *__inBuf;
	CyclicBuffer *__outBuf;

	char *__buffer;
	int __size;

	int __ret;
	int __flags;
};
