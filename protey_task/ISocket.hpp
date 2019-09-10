#pragma once

#include <cstddef>

#include <sys/types.h>

#ifdef _WIN32
#include <Winsock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#endif

const int DEFAULT_PORT = 12345;

struct ISocket
{
	virtual ~ISocket() = default;
	
	virtual ssize_t Send(const void *pBuffer, size_t nLength, int nFlags) = 0;
	virtual ssize_t SendTo(const void *pBuffer, size_t nLength, int nFlags, const struct sockaddr *pDestAddr, socklen_t AddrLen) = 0;
	
	virtual ssize_t Recv(void *pBuffer, size_t nLength, int nFlags) = 0;
	virtual ssize_t RecvFrom(void *pBuffer, size_t nLength, int nFlags, struct sockaddr *pSrcAddr, socklen_t *pAddrLen) = 0;
};