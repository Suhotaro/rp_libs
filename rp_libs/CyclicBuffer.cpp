#include "stdafx.h"
#include "Util.h"
#include "CyclicBuffer.h"

#include <Winsock2.h>

CyclicBuffer::CyclicBuffer()
{
	__signal = WSACreateEvent();
	EXITIFFALSE(__signal, "WSACreateEvent failed");
}

CyclicBuffer::~CyclicBuffer()
{
	WSACloseEvent(__signal);
}

const HANDLE& CyclicBuffer::Signal()
{
	return __signal;
}