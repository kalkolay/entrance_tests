#include <iostream>
#include <stdexcept>

#include <unistd.h>
#ifndef _WIN32
#include <netinet/in.h>
#endif

#include "SocketTCP.hpp"
#include "SocketUDP.hpp"

#include "IExecutionStrategy.hpp"
#include "CommonTypes.hpp"

class CUDPExecutionStrategy final : public IExecutionStrategy
{
public:
	void Execute() override
	{
		struct sockaddr_in ServAddr;

		ServAddr.sin_family = AF_INET;
		ServAddr.sin_port = htons(DEFAULT_PORT);
		ServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		
		ISocket *pSocket = new CSocketUDP();
		
		char Buffer[1024];
		scanf("%[^\n]%*c", &Buffer);

		pSocket->SendTo((const char*)Buffer, sizeof(Buffer) - 1,
			   MSG_CONFIRM, (const struct sockaddr *) &ServAddr,
			   sizeof(ServAddr));
		unsigned len;
		
		ssize_t n = pSocket->RecvFrom((char*)Buffer, sizeof(Buffer) - 1,
#ifdef _WIN32
					 0,
#else
					 MSG_WAITALL,
#endif
					 (struct sockaddr *) &ServAddr, &len);
		
		delete pSocket;
		
		std::cout << Buffer << std::endl;
	};
};

class CTCPExecutionStrategy final : public IExecutionStrategy
{
public:
	void Execute() override
	{
		struct sockaddr_in ServAddr;

		ServAddr.sin_family = AF_INET;
		ServAddr.sin_port = htons(DEFAULT_PORT);
		ServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		char Buffer[1024];
		scanf("%[^\n]%*c", &Buffer);

		CSocketTCPClient *pSocket = new CSocketTCPClient();

		pSocket->Connect((struct sockaddr*)(&ServAddr), sizeof(ServAddr));
		
		pSocket->Send(Buffer, sizeof(Buffer) - 1, MSG_NOSIGNAL);
		
		pSocket->Recv(Buffer, sizeof(Buffer) - 1, MSG_NOSIGNAL);

#ifdef _WIN32
		pSocket->Shutdown(SD_BOTH);
#else
		pSocket->Shutdown(SHUT_RDWR);
#endif
		
		delete pSocket;
		
		std::cout << Buffer << std::endl;
	};
};

IExecutionStrategy *ChooseSocketType(const char *socket)
{
	if(!strcmp(socket, "udp") || !strcmp(socket, "UDP"))
		return new CUDPExecutionStrategy();
	else if(!strcmp(socket, "tcp") || !strcmp(socket, "TCP"))
		return new CTCPExecutionStrategy();
	else
		throw std::runtime_error("Unsupported/undefined socket type!");
};

int main(int argc, char** argv)
{
#ifdef _WIN32
	static WSAData WinSockData{};
	int nResult{WSAStartup(MAKEWORD(2, 2), &WinSockData)};
	
	if(nResult != 0)
	{
		printf("WinSock startup failed: %d\n", nResult);
		return false;
	};
#endif

	int i;
	const char *sSocket{""};
	for(i = 0; i < argc; ++i)
		if(!strcmp(argv[i], "--socket_type"))
		{
			sSocket = argv[i + 1];
			break;
		};
	
	if(i == argc)
		throw std::runtime_error("Socket type is not specified!");
	
	IExecutionStrategy *pExecStrategy = ChooseSocketType(sSocket);
	pExecStrategy->Execute();

	delete pExecStrategy;

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
};
