#include "ClientUser.h"

CClientUser::CClientUser(void){

	memset(userName, 0, sizeof(char) * NAME_SIZE);
	memset(userPacket, 0, sizeof(char) * PACKET_SIZE);
	memset(&userAddr, 0, sizeof(userAddr));


	WSAStartup(MAKEWORD(2, 2), &wsaData);
	userSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	userAddr.sin_family = AF_INET;
	userAddr.sin_port = htons(PORT_NUM);
	userAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	packetHeader = PH_USER_CONNECT;
	userAddrSize = sizeof(userAddr);
}

CClientUser::~CClientUser(void){

}

void CClientUser::runUser(){

	system("cls");
	readSize = 0;
	cout<< "Enter your name : ";
	cin>> userName;

	memcpy(userPacket, &packetHeader, 4);
	readSize += 4;

	memcpy(userPacket + readSize, &userName, sizeof(char) * NAME_SIZE);
	readSize += sizeof(char) * NAME_SIZE;

	sendto(userSocket, userPacket, readSize, 0, 
		(SOCKADDR*)&userAddr, sizeof(userAddr));

	cout << "Waiting response from the server..." << endl;

	recvfrom(userSocket, userPacket, PACKET_SIZE - 1,0, 
		(SOCKADDR*)&userAddr, &userAddrSize);

	readSize = 0;
	memcpy(&packetHeader, userPacket, 4);
	readSize += 4;

	if(packetHeader == PH_SET_RULE) {

		packetHeader = PH_USER_CONNECT;
		memcpy(userPacket, &packetHeader, sizeof(int));

		system("cls");
		cout << "==== Setting Rule ====\n==== Enter below items ====" << endl;
		cout << "The number of users : ";
		int userCount;
		cin >> userCount;
		memcpy(userPacket + readSize, &userCount, sizeof(int));
		readSize += sizeof(int);

		cout << "The size of board : ";
		int boardSize;
		cin >> boardSize;
		memcpy(userPacket + readSize, &boardSize, sizeof(int));
		readSize += sizeof(int);

		cout << "The number of goal bingo : ";
		int goalBingo;
		cin >> goalBingo;
		memcpy(userPacket + readSize, &goalBingo, sizeof(int));
		readSize += sizeof(int);

		sendto(userSocket, userPacket, readSize, 0
			, (SOCKADDR*)&userAddr, sizeof(userAddr));
		recvfrom(userSocket, userPacket, PACKET_SIZE - 1, 0, 
			(SOCKADDR*)&userAddr, &userAddrSize);
	}

	readSize = 0;

	memcpy(&packetHeader, userPacket, sizeof(int));
	readSize += sizeof(int);

	if(packetHeader == PH_USER_INIT){

		int boardSize;
		memcpy(&boardSize, userPacket + readSize, sizeof(int));
		readSize += sizeof(int);

		int goalNumber;
		memcpy(&goalNumber, userPacket + readSize, sizeof(int));
		readSize += sizeof(int);

		userBoard = new CClientBoard(boardSize, goalNumber);
		userBoard->shuffleNumber();
		//packetHeader = PH_SELECT_NUMBER;
	}

	while(true) {

		if(packetHeader != PH_RETRY)
			wait();

		switch(packetHeader){

		case PH_SELECT_NUMBER :
			{
				selectNumber();

				break;
			}
		case PH_DISTRIBUTE_NUMBER :
			{
				distributeNumber();
				checkBingo();
				break;
			}
		case PH_WIN :
			{
				cout << "ÀÌ±è" << endl;
				break;
			}
		case PH_LOSE :
			{
				cout << "Áü" << endl;
				break;
			}
		}//switch			
	}//while
	WSACleanup();
}

void CClientUser::wait(){

	userBoard->printInfo();

	recvfrom(userSocket, userPacket, PACKET_SIZE - 1, 0
		, (SOCKADDR*)&userAddr, &userAddrSize); 

	readSize = 0;
	memcpy(&packetHeader, userPacket, sizeof(int));
	readSize += sizeof(int);
};

void CClientUser::selectNumber(){

	while(true) {

		cout << "\nEnter the number which you want to select : ";
		int number;
		cin >> number;

		if(!userBoard->checkRepetition(number))
			continue;

		memcpy(userPacket + readSize, &number, sizeof(int));
		readSize += sizeof(int);

		sendto(userSocket, userPacket, readSize, 0
			, (SOCKADDR*)&userAddr, sizeof(userAddr));
		break;
	}
}

void CClientUser::distributeNumber(){

	int number;
	memcpy(&number, userPacket + readSize, sizeof(int));

	userBoard->setCurNumber(number);
	userBoard->distributeNumber();
}

void CClientUser::checkBingo(){

	if(userBoard->checkBingo()){
		
		packetHeader = PH_WIN;
		readSize = 0;
		memcpy(userPacket, &packetHeader, sizeof(int));
		readSize += sizeof(int);

		//ºù°íÆÇ Á¤º¸ Àü¼Û ¹Ì±¸Çö

		sendto(userSocket, userPacket, readSize, 0
			, (SOCKADDR*)&userAddr, sizeof(userAddr));
	}
	else{

		packetHeader = PH_DISTRIBUTE_NUMBER;
		readSize = 0;
		memcpy(userPacket, &packetHeader, sizeof(int));
		readSize += sizeof(int);
		sendto(userSocket, userPacket, readSize, 0
			, (SOCKADDR*)&userAddr, sizeof(userAddr));
	}
}