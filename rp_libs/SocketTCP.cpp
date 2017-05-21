#include "stdafx.h"
#include "SocketTCP.h"
#include "NetUtil.h"
#include <string>

rpTCPServerSocket::rpTCPServerSocket(SOCKET socket)
	: rpParrent(socket)
{}

rpTCPClientSocket::rpTCPClientSocket(std::string address, int port,
									 int af, int type, int protocol)
	: rpParrent(af, type, protocol), __address(__address), __port(port)
{}

bool rpTCPClientSocket::Init()
{
	__socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	NETRETVALIFFALSE(__socket, false, "create socket");

	struct sockaddr_in server;
	memset(&server, 0, sizeof(struct sockaddr_in));

	server.sin_family = __af;
	server.sin_port = htons(__port);
	server.sin_addr.S_un.S_addr = inet_addr(__address.c_str());
	if (server.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		struct hostent *host = gethostbyname(__address.c_str());
		NETRETVALIFFALSE(host, false, "resolve server %s", __address.c_str());

		//$todo: iterate through all
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
	}

	int ret = connect(__socket, (struct sockaddr *)&server, sizeof(server));
	NETRETVALIFFALSE(ret != SOCKET_ERROR, false, "connect to server %s", __address.c_str());

	return true;
}
