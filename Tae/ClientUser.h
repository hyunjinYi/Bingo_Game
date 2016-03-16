#pragma once
#include"value.h"
#include"ClientBoard.h"

class CClientUser
{
public:
	CClientUser(void);
	~CClientUser(void);
public:
	void runUser();
	void wait();
	void selectNumber();
	void distributeNumber();
	void checkBingo();

private:
	char						userName[NAME_SIZE];
	CClientBoard*		userBoard;

	WSADATA			wsaData;
	SOCKET				userSocket;
	SOCKADDR_IN		userAddr;
	int						userAddrSize;
	char						userPacket[PACKET_SIZE];
	int						packetHeader;
	int						readSize;

};

