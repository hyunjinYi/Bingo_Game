#pragma once
#include"value.h"

class CClientBoard
{
public:
	CClientBoard(int size, int number);
	~CClientBoard(void);

public:
	void shuffleNumber();
	void printInfo();
	void distributeNumber();
	void setCurNumber(int number);
	bool checkRepetition(int number);
	bool checkBingo();

private:
	int** board;
	int** markBoard;
	int** positionNum;
	int* checkRow;
	int* checkCol;
	int* usedList;
	int* flag;
	int listIdx;

	int boardSize;
	int curBingo;
	int goalBingo;
	int bingoSize;
	int curNum;
	bool userState;
};

