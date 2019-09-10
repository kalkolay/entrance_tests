#pragma once

#include "BaseSocket.hpp"

class CSocketTCP : public CBaseSocket
{
public:
	CSocketTCP();
	virtual ~CSocketTCP() = default;
};

class CSocketTCPClient : public CSocketTCP
{
public:
	CSocketTCPClient() : CSocketTCP(){}
	CSocketTCPClient(int nHandle){this->nSocket = nHandle;}
	
	int Connect(const struct sockaddr *pAddr, socklen_t AddrLen);
	int Shutdown(int nHow);
};

class CSocketTCPServer : public CSocketTCP
{
public:
	CSocketTCPServer(int nPort);
	
	CSocketTCPClient *Accept(struct sockaddr *pAddr, socklen_t *pAddrLen);
};