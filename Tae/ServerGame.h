#pragma once
#include"value.h"
#include"ServerUser.h"

class CServerGame
{
public:
	CServerGame(void);
	~CServerGame(void);

public:
	void initGame(int number);
	void runGame();
	bool isUserFull();
	void distNumber(int selNumber);
	void checkWinner();
	
	CServerUser* insertUser();

private:
	CServerUser*		userList[USER_SIZE];//
	CServerUser*        winnerList[USER_SIZE];
	CServerUser*		loserList[USER_SIZE];
	int*						usedNumber;
	int						userCount;//
	int						curUserCount;//
	int						goalBingo;
	int						boardSize;
	int						listIndex;
	int						winnerCount;
	int						loserCount;
	//server part
	WSADATA			wsaData;
	SOCKET				servSocket;
	SOCKADDR_IN		servAddr;
	char						servPacket[PACKET_SIZE];
	int						packetLen;
	int						packetHeader;
	int						readSize;
	//client part
	SOCKADDR_IN		clntAddr; // hold user socket address
	int						clntAddrSize;

};

