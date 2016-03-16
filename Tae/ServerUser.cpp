#include "ServerUser.h"

CServerUser::CServerUser(void)
{
	memset(userName, 0, sizeof(char) * NAME_SIZE);
}

CServerUser::~CServerUser(void)
{
}

void CServerUser::initUser(int boardSize){
	userBoard = new CServerBoard(boardSize);
}

void CServerUser::setUserName(char* name){
	
	strcpy_s(userName, name);
}

void CServerUser::setUserSockAddr(SOCKADDR_IN sockAddr){

	userSockAddr = sockAddr;
}

char* CServerUser::getUserName(){

	return userName;
}

SOCKADDR_IN CServerUser::getUserSockAddr(){

	return userSockAddr;
}
