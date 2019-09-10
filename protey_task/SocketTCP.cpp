#ifndef _WIN32
#include <netinet/in.h>
#endif

#include "SocketTCP.hpp"

CSocketTCP::CSocketTCP()
{
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
};

int CSocketTCPClient::Connect(const struct sockaddr *pAddr, socklen_t AddrLen)
{
	return connect(nSocket, pAddr, AddrLen);
};

int CSocketTCPClient::Shutdown(int nHow)
{
	return shutdown(nSocket, nHow);
};

CSocketTCPServer::CSocketTCPServer(int nPort) : CSocketTCP()
{
	struct sockaddr_in SockAddr;

	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(nPort);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(nSocket, (struct sockaddr*)(&SockAddr), sizeof(SockAddr));
	
	listen(nSocket, SOMAXCONN);
};

CSocketTCPClient *CSocketTCPServer::Accept(struct sockaddr *pAddr, socklen_t *pAddrLen)
{
	int nClientSocket = accept(nSocket, pAddr, pAddrLen);
	
	if(nClientSocket < 0)
	{
		close(nClientSocket);
		return nullptr;
	};
	
	return new CSocketTCPClient(nClientSocket);
};