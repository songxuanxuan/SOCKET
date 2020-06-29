#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

#define BUFSIZE 255

using std::cout;
using std::cin;
using std::endl;

class TCPClient
{
	void debuglog(const char* strLog);
	int initialization();
	SOCKET CreateConnect(int port, const char *addr);
	BOOL SendServer(SOCKET hServer);
	BOOL CloseCon(SOCKET hServer);
public:
	TCPClient();
};

