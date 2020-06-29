#include "TCPClient.h"

void TCPClient::debuglog(const char* strLog)
{
	cout << strLog << WSAGetLastError() << endl;
}

int TCPClient::initialization()
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

SOCKET TCPClient::CreateConnect(int port, const char *addr)
{
	SOCKET hServer = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saServer = { AF_INET ,htons(port) };
	if (inet_pton(AF_INET, addr, &saServer.sin_addr) == 0)
	{
		debuglog("CreateConnect::inet_pton error:  ");
		return INVALID_SOCKET;
	}

	if (connect(hServer, (sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR)
	{
		debuglog("CreateConnect::connect error:  ");
		return INVALID_SOCKET;
	}
	else
	{
		cout << "���ӳɹ� -- " << inet_ntoa(saServer.sin_addr) << endl;
	}
	return hServer;
}

BOOL TCPClient::SendServer(SOCKET hServer)
{
	int nRet = 0;
	char sBuf[BUFSIZE];
	do
	{
		memset(sBuf, 0, BUFSIZE);
		cout << "���룺 " << endl;
		cin >> sBuf;
		nRet = send(hServer, sBuf, BUFSIZE, 0);
		if (nRet == SOCKET_ERROR)
		{
			debuglog("SendServer::send error: ");
			break;
		}
		else if (nRet > 0)
		{
			recv(hServer, sBuf, BUFSIZE, 0);
			cout << "���ͳɹ�" << sBuf << endl;
		}
	} while (nRet>0);

	return 1;
}

BOOL TCPClient::CloseCon(SOCKET hServer)
{
	if (shutdown(hServer, SD_SEND) == SOCKET_ERROR)
	{
		debuglog("CloseConnection::shutdown error:  ");
		return false;
	}
	char buf[BUFSIZE];
	int nRet;
	do
	{
		nRet = recv(hServer, buf, BUFSIZE, 0);
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
	} while (nRet != 0);
	if (closesocket(hServer) == SOCKET_ERROR)
	{
		debuglog("CloseConnection::closesocket error:  ");
		return false;
	}
	return true;
}

TCPClient::TCPClient()
{
	SOCKET hServer;
	initialization();
	hServer = CreateConnect(8888, "127.0.0.1");
	SendServer(hServer);
	CloseCon(hServer);
	WSACleanup();
}
