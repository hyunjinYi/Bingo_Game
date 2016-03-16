#pragma once
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#include<WinSock2.h>
#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<time.h>

#define NAME_SIZE 20
#define PACKET_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define PORT_NUM 8000
#define	 USER_SIZE 3

enum PACKET_HEADER{

	PH_USER_CONNECT,
	PH_SET_RULE,
	PH_USER_INIT,
	PH_SELECT_NUMBER,
	PH_DISTRIBUTE_NUMBER,
	PH_RETRY,
	PH_WIN,
	PH_LOSE,
	PH_NULL
};

enum MODE_FLAG{

	MF_SERVER = 1,
	MF_CLIENT,
	MF_EXIT
};