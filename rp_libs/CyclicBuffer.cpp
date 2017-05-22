#include "stdafx.h"
#include "Util.h"
#include "CyclicBuffer.h"

#include <Winsock2.h>
#include <string.h>

CyclicBuffer::CyclicBuffer()
{
	__signal = WSACreateEvent();
	EXITIFFALSE(__signal, "WSACreateEvent failed");
}

CyclicBuffer::~CyclicBuffer()
{
	WSACloseEvent(__signal);
}

void CyclicBuffer::Add(char *buffer, int size)
{
	__buffer += buffer;
	WSASetEvent(__signal);
}

void CyclicBuffer::Pop(char *buffer, int *size, int maxBufferSize)
{
	if (maxBufferSize < __buffer.size())
	{
		*size = maxBufferSize;
		strncpy_s(buffer, maxBufferSize, __buffer.c_str(), maxBufferSize);
		__buffer.erase(0, maxBufferSize);
	}
	else
	{
		*size = __buffer.size();
		strncpy_s(buffer, maxBufferSize, __buffer.c_str(), __buffer.size());
		__buffer.erase(0, __buffer.size());
	}
}

const HANDLE& CyclicBuffer::Signal()
{
	return __signal;
}