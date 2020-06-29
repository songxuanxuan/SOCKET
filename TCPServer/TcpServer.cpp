#include "TcpServer.h"


void TcpServer::debuglog(const char* strLog)
{
	cout << strLog << WSAGetLastError() << endl;
}

int TcpServer::initialization()
{
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		//cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
		debuglog("WSAStartupʧ��!--");
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		debuglog("WSAStartup�汾����!--");
		WSACleanup();
	}
	return 0;
}

SOCKET TcpServer::Bind_Listen(int nBacklog)
{
	SOCKET hServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hServer == INVALID_SOCKET)
	{
		debuglog("Bind_Listen error:  ");
		return INVALID_SOCKET;
	}

	struct sockaddr_in serverAddr = { AF_INET,htons(MYTCPPORT) };
	if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) == 0)
	{
		debuglog("Bind_Listen::inet_pton error:  ");
		return INVALID_SOCKET;
	}

	if (bind(hServer, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		debuglog("Bind_Listen::bind error:  ");
		closesocket(hServer);
		return INVALID_SOCKET;
	}
	if (listen(hServer, nBacklog) == SOCKET_ERROR)
	{
		debuglog("Bind_Listen::listen error:  ");
		closesocket(hServer);
		return INVALID_SOCKET;
	}
	return hServer;
}

SOCKET TcpServer::AcceptConnection(SOCKET hSocket)
{
	sockaddr_in saClientIn;
	int addrLen = sizeof(saClientIn);
	SOCKET hClientIn = accept(hSocket, (sockaddr*)&saClientIn, &addrLen);
	if (hClientIn == INVALID_SOCKET) 
	{
		debuglog("AcceptConnection::accept error:  ");
		return INVALID_SOCKET;
	}
	else
	{
		cout << "���ӳɹ� -- " << inet_ntoa( saClientIn.sin_addr) << endl;
	}
	return hClientIn;
}

BOOL TcpServer::ClientConFun(SOCKET sd)
{
	char szBuf[BUFSIZE];
	int nRet;
	do
	{
		nRet = recv(sd, szBuf, BUFSIZE, 0);
		if (nRet == SOCKET_ERROR)
		{
			debuglog("AcceptConnection::accept error:  ");
			return false;
		}
		else if(nRet >0)
		{
			cout << "�յ�һ����Ϣ: " << szBuf << endl;
			int nSend = 0;
			while (nSend<nRet)
			{
				int nTemp = send(sd, &szBuf[nSend], nRet - nSend, 0);
				if (nTemp>0)
				{
					nSend += nTemp;
				}
				else if (nTemp == SOCKET_ERROR)
				{
					debuglog("AcceptConnection::�ش� error:  ");
					break;
				}
				else
				{
					debuglog("AcceptConnection::send����ر� error:  ");
					break;
				}
			}
		}
	} while (nRet>0);


	return 1;
}

BOOL TcpServer::CloseConnection(SOCKET sd)
{

	if(shutdown(sd, SD_SEND) == SOCKET_ERROR)
	{
		debuglog("CloseConnection::shutdown error:  ");
		return false;
	}
	char buf[BUFSIZE];
	int nRet;
	do
	{
		nRet = recv(sd, buf, BUFSIZE, 0);
		if (nRet == SOCKET_ERROR)
		{
			debuglog("CloseConnection::recv error:  ");
			return false;
		}
		else if (nRet > 0)
		{
			debuglog("CloseConnection::recv error:���ݽ��ܲ���ȫ  ");
			return false;

		}
	} while (nRet!=0);
	if (closesocket(sd)==SOCKET_ERROR)
	{
		debuglog("CloseConnection::closesocket error:  ");
		return false;
	}
	return true;
}
 DWORD WINAPI ServerThreadFun(LPVOID p)
{
	if (ClientConFun((SOCKET)p) == false)
	{
		return 0;
	}
	if (CloseConnection((SOCKET)p) == false)
	{
		return 0;
	}
	return 1;
}
TcpServer::TcpServer(int nBacklog)
{
	SOCKET hServer;
	SOCKET hClientIn;
	initialization();
	hServer = Bind_Listen(nBacklog);
	while (true)
	{
		cout << "�ȴ����ӡ�����" << endl;
		hClientIn = AcceptConnection(hServer);
		HANDLE hThread = CreateThread(NULL, 0, ServerThreadFun, (LPVOID)hClientIn, 0, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
	}
	if (closesocket(hServer) == SOCKET_ERROR)
	{
		debuglog("TcpServer::closesocket error:  ");
	}
	WSACleanup();
	
}
