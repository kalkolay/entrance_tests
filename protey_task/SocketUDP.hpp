#pragma once

#include "BaseSocket.hpp"

class CSocketUDP : public CBaseSocket
{
public:
	CSocketUDP();
	virtual ~CSocketUDP() = default;
};

class CSocketUDPServer : public CSocketUDP
{
public:
	CSocketUDPServer(int nPort);
};