#pragma once
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#define MYTCPPORT 8888
#define BUFSIZE 255

using std::cout;
using std::cin;
using std::endl;

class TcpServer
{


	void debuglog(const char *strLog);
	int initialization();
	SOCKET Bind_Listen(int nBacklog);
	SOCKET AcceptConnection(SOCKET hSocket);
	BOOL ClientConFun(SOCKET sd);
	BOOL CloseConnection(SOCKET sd);
public:
	TcpServer(int nBacklog);
};

