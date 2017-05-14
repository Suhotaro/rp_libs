#pragma once

#include "Socket.h"

class CyclicBuffer;

class rpSelector
{
public:
	rpSelector(CyclicBuffer *inBuf, CyclicBuffer *outBuf);
	~rpSelector();

	bool Init();
	void Update();
	int Errors();

protected:
	rpSocket *__socket;
	CyclicBuffer *__inBuf;
	CyclicBuffer *__outBuf;

	fd_set fdread;
	fd_set fdwrite;
	int ret;
};
