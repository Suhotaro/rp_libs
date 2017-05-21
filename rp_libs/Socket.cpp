#include "stdafx.h"
#include "Socket.h"

rpSocketBase::rpSocketBase(int af, int type, int protocol)
	: __af(af), __type(type), __protocol(protocol)
{}

rpSocketBase::rpSocketBase(SOCKET socket) : __socket(socket)
{}

rpSocketBase::~rpSocketBase() {
	closesocket(__socket);
}

SOCKET& rpSocketBase::Handle() {
	return __socket; 
}

rpSocket::rpSocket(int af, int type, int protocol) 
	: rpParrent(af, type, protocol)
{}

rpSocket::rpSocket(SOCKET socket)
	: rpParrent(socket) 
{}

int rpSocket::Write(const char *buf, size_t len, int flags) {
	return send(__socket, buf, len, flags);
}

int rpSocket::Read(char *buf, size_t len, int flags) {
	return recv(__socket, buf, len, flags);
}

bool rpSocket::Init() {
	return true;
}
