#pragma once

#define NETRETIFTRUE(statement, msg, ...)	\
if (statement)								\
{											\
	printf( msg, ##__VA_ARGS__);			\
	return;									\
}

#define NETRETVALIFTRUE(statement, ret, msg, ...)					\
if (statement)														\
{																	\
	printf("ERROR[%d]:" msg, WSAGetLastError(), ##__VA_ARGS__);		\
	return ret;														\
}