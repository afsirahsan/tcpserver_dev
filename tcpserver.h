#include"stdafx.h"
class ctcplistener;
typedef void(*MessageReceivedHandler)(ctcplistener* ptrlistener, string msg);
class ctcplistener
{
public:
	ctcplistener(string ipaddress, int port, MessageReceivedHandler phandler) :m_ipaddress(ipaddress),
		m_iport(port), m_phandlersocket(phandler)
	{

	}
	~ctcplistener(){

	}
	//initialize the socket
	void init();
	//Main processing loop
	void Run();
	//send the message back to client
	void sendmessagetoclient();
private:
	int m_iport;
	string m_ipaddress;
	MessageReceivedHandler m_phandlersocket;
	int CreateSocket();
	SOCKET waitfortheconnection(SOCKET socket);

};