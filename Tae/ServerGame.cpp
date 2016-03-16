#include "ServerGame.h"

CServerGame::CServerGame(void) {

	memset(userList, 0, sizeof(CServerUser) * USER_SIZE);
	memset(winnerList, 0, sizeof(CServerUser) * USER_SIZE);
	memset(loserList, 0, sizeof(CServerUser) * USER_SIZE);
	usedNumber = NULL;
	userCount = 0;
	curUserCount = 0;
	listIndex = -1;
	clntAddrSize = sizeof(clntAddr);
	winnerCount = -1;
	loserCount = -1;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	servSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT_NUM);
	servAddr.sin_addr.s_addr =htonl(INADDR_ANY);

	bind(servSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));
}

CServerGame::~CServerGame(void) {

}

void CServerGame::initGame(int number){

	userList[number]->initUser(boardSize);
	usedNumber = new int[boardSize * boardSize];
	memset(usedNumber, 0, sizeof(int) * boardSize * boardSize);
	listIndex = -1;
}

void CServerGame::runGame(){

	while(true){ 

		cout<< "==== Bingo Server Event ===="<<endl;

		packetLen = recvfrom(servSocket, servPacket, PACKET_SIZE - 1, 0, 
			(SOCKADDR*)&clntAddr, &clntAddrSize); 

		readSize = 0;

		memcpy(&packetHeader, servPacket, 4);// read packet header
		readSize += 4;

		//when the first user is connected
		if(packetHeader == PH_USER_CONNECT) {

			CServerUser* user = new CServerUser;

			userList[curUserCount] = user;
			userList[curUserCount]->setUserSockAddr(clntAddr);

			char name[NAME_SIZE] = {0};
			memcpy(name, servPacket + readSize, sizeof(char) * NAME_SIZE);

			readSize += (sizeof(char) * NAME_SIZE);
			userList[curUserCount]->setUserName(name);

			cout << "User '" << userList[curUserCount]->getUserName() << "' is connected"<<endl;

			curUserCount++;

			if(curUserCount == 1) {

				readSize = 0;
				packetHeader = PH_SET_RULE;
				memcpy(servPacket, &packetHeader, sizeof(int));
				readSize += sizeof(int);
				sendto(servSocket, servPacket, readSize, 0
					, (SOCKADDR*)&userList[curUserCount - 1]->getUserSockAddr(), sizeof(SOCKADDR_IN));

				recvfrom(servSocket, servPacket, PACKET_SIZE - 1, 0, 
					(SOCKADDR*)&userList[curUserCount - 1]->getUserSockAddr(), &clntAddrSize);

				readSize = 0;
				memcpy(&packetHeader, servPacket, sizeof(int));
				readSize += sizeof(int);

				memcpy(&userCount, servPacket + readSize, sizeof(int));
				readSize += sizeof(int);

				memcpy(&boardSize, servPacket + readSize, sizeof(int));
				readSize += sizeof(int);

				memcpy(&goalBingo, servPacket + readSize, sizeof(int));
				readSize += sizeof(int);
			}
		}

		if(!isUserFull())
			continue;

		if(packetHeader == PH_USER_CONNECT){
			packetHeader = PH_USER_INIT;

			readSize = 0;
			memcpy(servPacket, &packetHeader, sizeof(int));
			readSize += sizeof(int);

			memcpy(servPacket + readSize, &boardSize, sizeof(int));
			readSize += sizeof(int);

			memcpy(servPacket + readSize, &goalBingo, sizeof(int));
			readSize += sizeof(int);

			for(int i = 0; i < userCount; i++){

				initGame(i);
				sendto(servSocket, servPacket, readSize, 0
					, (SOCKADDR*)&userList[i]->getUserSockAddr(), sizeof(SOCKADDR_IN));
			}
		}

		for(int i = 0; i < userCount; i++){

			readSize = 0;
			packetHeader = PH_SELECT_NUMBER;
			memcpy(servPacket, &packetHeader, sizeof(int));
			readSize += sizeof(int);

			sendto(servSocket, servPacket, readSize, 0
				, (SOCKADDR*)&userList[i]->getUserSockAddr(), sizeof(SOCKADDR_IN));
			recvfrom(servSocket, servPacket, PACKET_SIZE - 1, 0
						, (SOCKADDR*)&userList[i]->getUserSockAddr(), &clntAddrSize);
			
			readSize = 0;
			memcpy(&packetHeader, servPacket, sizeof(int));
			readSize += sizeof(int);
			
			switch(packetHeader){

			case PH_RETRY :
				{
					//필요한가?
					break;
				}
			case PH_SELECT_NUMBER:
				{
					memcpy(&usedNumber[++listIndex], servPacket + readSize, sizeof(int));

					readSize = 0;
					packetHeader = PH_DISTRIBUTE_NUMBER;
					memcpy(servPacket, &packetHeader, sizeof(int));
					readSize += sizeof(int);

					memcpy(servPacket + readSize, &usedNumber[listIndex], sizeof(int));
					readSize += sizeof(int);

					for(int i = 0; i < userCount; i++) {

						sendto(servSocket, servPacket, PACKET_SIZE - 1, 0
							, (SOCKADDR*)&userList[i]->getUserSockAddr(), sizeof(SOCKADDR_IN));
						recvfrom(servSocket, servPacket, PACKET_SIZE - 1, 0
							, (SOCKADDR*)&userList[i]->getUserSockAddr(), &clntAddrSize);
						
						readSize = 0;
						memcpy(&packetHeader, servPacket, sizeof(int));
						readSize += 4;
						
						if(packetHeader == PH_WIN)
							winnerList[++winnerCount] = userList[i];
						else{
							if(winnerCount > -1)
								loserList[++loserCount] = userList[i];
						}
					}
					break;
				}
			}

			checkWinner();

			if(i == userCount - 1)
				i = -1;
		}
	}
	WSACleanup();//소켓 연결 해제
}

bool CServerGame::isUserFull(){

	if(curUserCount == userCount)
		return true;

	return false;
}

void CServerGame::checkWinner(){

	if(winnerCount > -1) {
		for(int i = 0; i <= winnerCount; i++){

			packetHeader = PH_WIN;
			readSize = 0;
			memcpy(servPacket, &packetHeader, sizeof(int));
			readSize += sizeof(int);

			sendto(servSocket, servPacket, readSize, 0
				, (SOCKADDR*)&winnerList[i]->getUserSockAddr(), sizeof(SOCKADDR_IN));
		}

		for(int i = 0; i <= loserCount; i++){

			packetHeader = PH_LOSE;
			readSize = 0;
			memcpy(servPacket, &packetHeader, sizeof(int));
			readSize += sizeof(int);

			sendto(servSocket, servPacket, readSize, 0
				, (SOCKADDR*)&winnerList[i]->getUserSockAddr(), sizeof(SOCKADDR_IN));
		}
		//각 유저에게 누가 승 패 여부와 누가 승리했는지 알려줘야함
		//서버 정상 종료 루틴 필요
		//메모리 해제
		
		cout << "==== Result ====" << endl;
		cout << "Winner :";
		for(int i = 0; i < winnerCount; i++)
			cout << " " << winnerList[i];
	
		cout << endl;

		for(int i = 0; i < loserCount; i++)
			cout << " " << loserList[i];

		cout << endl;
	}
}