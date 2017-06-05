#include "stdafx.h"

#include "LibInit.h"

#include <stdio.h>
#include <winsock2.h>

bool rpLibInit()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("ERROR: WSAStartup failed %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

void RP_LIBS_API rpLibDone()
{
	WSACleanup();
}