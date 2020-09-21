// tcpserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tcpserver.h"

//initialize winsock
void ctcplistener::init()
{
	WSADATA wsadata;
	int nresult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (nresult != 0)
	{
		cout << "Initialization of the socket has failed\n";
		return;
	}
}
int ctcplistener::CreateSocket()
{
	SOCKET nsocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nsocket == INVALID_SOCKET)
	{
		cout << "unable to create socket\n";
		return -1;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_iport);
	inet_pton(AF_INET, m_ipaddress.c_str(), &hint.sin_addr);
	int nresult = bind(nsocket, (sockaddr*)&hint, sizeof(&hint));
	if (nresult != 0)
	{
		int listening = listen(nsocket, SOMAXCONN);
		if (listening == SOCKET_ERROR)
		{
			return -1;
		}
		cout << "\n socket is successfully bind";
	}
	else
	{
		return -1;
	}
	return nsocket;
}

SOCKET ctcplistener::waitfortheconnection(SOCKET nsocket)
{
	SOCKET nclient = accept(nsocket, NULL, NULL);
	return nclient;
}

void ctcplistener::Run()
{
	char buf[4096];

	while (true)
	{
		//createsocket
		SOCKET listening = CreateSocket();
		//wait for the connection
		SOCKET listenclient = waitfortheconnection(listening);
		closesocket(listening);
		//receive and send
		int bytesreceived = recv(listenclient, buf, 4096, 0);
		if (bytesreceived > 0)
		{
			//cout << "\n Message received at server side =" << string(buf).c_str();
			if (m_phandlersocket != NULL)
			{
				m_phandlersocket(this, string(buf).c_str());
			}
			memset(buf, 0, sizeof(buf));
			string str = "Message sent from client side";
			strcpy_s(buf, str.c_str());
			int result = send(listenclient, buf, sizeof(buf), 0);
			
			if (result == 0)
			{
				cout << "\n Message sent to client\n";
			}
		}
		else
		{
			cout << "\n No message received from client";
			closesocket(listenclient);
			break;
		}
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	auto fnsendmessage = [](ctcplistener* ptrlistener, string str)
	{
		cout << "\n message received at server side\n";
		cout << str.c_str();
	};
	ctcplistener * ptrhander = new ctcplistener("127.0.0.1", 54000, fnsendmessage);
	ptrhander->init();
	ptrhander->Run();
	return 0;
}

