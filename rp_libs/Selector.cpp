#include "Selector.h"
#include "SocketTCP.h"

#include <stdio.h>

rpSelector::rpSelector(CyclicBuffer *inBuf, CyclicBuffer *outBuf)
: __inBuf(inBuf), __outBuf(outBuf)
{
	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);
}

rpSelector::~rpSelector()
{}

bool rpSelector::Init()
{
	bool ret = __socket->Init();
	if (ret)
	{
		printf("ERROR: Selector Init\n");
		return false;
	}

	FD_SET(__socket->Socket(), &fdread);
	FD_SET(__socket->Socket(), &fdwrite);

	return true;
}

void rpSelector::Update()
{
	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);

	int ret = select(0, &fdread, nullptr, nullptr, nullptr);
	switch (ret)
	{
	case SOCKET_ERROR:
		//TODO: drop the program?
		printf("ERROR: Selector Init\n");
		return;

	case 0:
		printf("WARNING: timeout on select\n");
		break;

	default:
		if (FD_ISSET(__socket->Socket(), &fdread))
		{
			//do read
		}	
	}	
}

int rpSelector::Errors()
{}

