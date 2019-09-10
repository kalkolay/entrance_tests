#ifndef _WIN32
#include <netinet/in.h>
#endif

#include "SocketUDP.hpp"
#include <cstring>

CSocketUDP::CSocketUDP()
{
	nSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
};

CSocketUDPServer::CSocketUDPServer(int nPort) : CSocketUDP()
{
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(nPort);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(nSocket, (const struct sockaddr*)(&servaddr), sizeof(servaddr));
};
