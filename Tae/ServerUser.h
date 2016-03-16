#pragma once
#include"value.h"
#include"ServerBoard.h"

class CServerUser
{
public:
	CServerUser(void);
	~CServerUser(void);

public:
	void initUser(int boardSize);

	void setUserName(char* name);
	void setUserSockAddr(SOCKADDR_IN sockAddr);

	char* getUserName();
	SOCKADDR_IN getUserSockAddr();

private:
	char						userName[NAME_SIZE];
	CServerBoard*		userBoard;
	SOCKADDR_IN		userSockAddr;
	int						userSockAddrSize;
};

