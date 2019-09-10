#include <iostream>
#include <vector>
#include <sstream>
#include <strstream>
#include <algorithm>
#include <stdexcept>

#include <sys/types.h>

#include <unistd.h>
#ifndef _WIN32
#include <netinet/in.h>
#endif

#include "SocketTCP.hpp"
#include "SocketUDP.hpp"

#include "IExecutionStrategy.hpp"
#include "CommonTypes.hpp"

std::string serverOutput(char* input)
{
    std::vector<unsigned> vec;
    
    std::string str(input);
    std::istringstream iss(str);
    char c;
    while (iss >> c)
        if ( std::isdigit(c) )
        {
            iss.putback(c);
            unsigned tmp;
            iss >> tmp;
            vec.push_back(tmp);
        }
    
    std::sort( vec.begin(), vec.end() );
    
    std::ostrstream result;
    for (auto i : vec)
        result << i << ' ';
    
    unsigned sum = 0;
    for (auto i : vec)
        sum += i;
    result << sum << std::ends;
    
    return result.str();
}

class CUDPExecutionStrategy final : public IExecutionStrategy
{
public:
	void Execute() override
	{
		char Buffer[1024] = { 0 };
		
		struct sockaddr_in cliaddr;

		memset(&cliaddr, 0, sizeof(cliaddr)); 
		
		ISocket *pServSock = new CSocketUDPServer(DEFAULT_PORT);
		
		while (true)
		{
			unsigned len;
			
			ssize_t n = pServSock->RecvFrom((char*)Buffer, sizeof(Buffer) - 1,
#ifdef _WIN32
						 0,
#else
						 MSG_WAITALL,
#endif
						 (struct sockaddr*)(&cliaddr), &len);
			
			if(n <= 0)
				continue;
			
			std::string tmp = serverOutput(Buffer);
			strcpy( Buffer, tmp.c_str() );
			
			pServSock->SendTo((const char*)Buffer, sizeof(Buffer) - 1,
				   MSG_CONFIRM, (struct sockaddr*)(&cliaddr), len);
		};
		
		delete pServSock;
	};
};

class CTCPExecutionStrategy final : public IExecutionStrategy
{
public:
	void Execute() override
	{
		char Buffer[1024] = { 0 };
		
		CSocketTCPServer *pMasterSocket = new CSocketTCPServer(DEFAULT_PORT);
		
		while (true)
		{
			CSocketTCPClient *pSlaveSocket = pMasterSocket->Accept(nullptr, nullptr);

			if(!pSlaveSocket)
				continue;
			
			pSlaveSocket->Recv(Buffer, sizeof(Buffer) - 1, MSG_NOSIGNAL);
			
			std::string tmp = serverOutput(Buffer);
			strcpy( Buffer, tmp.c_str() );
			
			pSlaveSocket->Send(Buffer, sizeof(Buffer) - 1, MSG_NOSIGNAL);
#ifdef _WIN32
			pSlaveSocket->Shutdown(SD_BOTH);
#else
			pSlaveSocket->Shutdown(SHUT_RDWR);
#endif
			delete pSlaveSocket;
		};
	};
};

IExecutionStrategy *ChooseSocketType(const char *socket)
{
	if(!strcmp(socket, "udp"))
		return new CUDPExecutionStrategy();
	else if(!strcmp(socket, "tcp"))
		return new CTCPExecutionStrategy();
	else
		throw std::runtime_error("Unsupported/undefined socket type!");
};

int main(int argc, char **argv)
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
