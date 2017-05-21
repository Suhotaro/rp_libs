#pragma once

#include "stdafx.h"

class CyclicBuffer
{
public:
	CyclicBuffer();
	~CyclicBuffer();

	// TODO: make signal being class holding either cond var or HANDLE
	const HANDLE& Signal();
private:
	HANDLE __signal;
};