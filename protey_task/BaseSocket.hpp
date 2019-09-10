#pragma once

#include <cstdio>
#include <unistd.h>

#include "ISocket.hpp"

class CBaseSocket : public ISocket
{
public:
	virtual ~CBaseSocket()
	{
#ifdef _WIN32
		closesocket(nSocket);
#else
		close(nSocket);
#endif
	};

	ssize_t Send(const void *pBuffer, size_t nLength, int nFlags) override final
	{
#ifdef _WIN32
		return send(nSocket, (const char*)pBuffer, nLength, nFlags);
#else
		return send(nSocket, pBuffer, nLength, nFlags);
#endif
	};

	ssize_t SendTo(const void *pBuffer, size_t nLength, int nFlags, const struct sockaddr *pDestAddr, socklen_t AddrLen) override final
	{
#ifdef _WIN32
		return sendto(nSocket, (const char *)pBuffer, nLength, nFlags, pDestAddr, AddrLen);
#else
		return sendto(nSocket, pBuffer, nLength, nFlags, pDestAddr, AddrLen);
#endif
	};

	ssize_t Recv(void *pBuffer, size_t nLength, int nFlags) override final
	{
#ifdef _WIN32
		return recv(nSocket, (char*)pBuffer, nLength, nFlags);
#else
		return recv(nSocket, pBuffer, nLength, nFlags);
#endif
	};

	ssize_t RecvFrom(void *pBuffer, size_t nLength, int nFlags, struct sockaddr *pSrcAddr, socklen_t *pAddrLen) override final
	{
#ifdef _WIN32
		return recvfrom(nSocket, (char*)pBuffer, nLength, nFlags, pSrcAddr, pAddrLen);
#else
		return recvfrom(nSocket, pBuffer, nLength, nFlags, pSrcAddr, pAddrLen);
#endif
	};
protected:
	int nSocket{-1};
};
