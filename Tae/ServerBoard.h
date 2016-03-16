#pragma once
#include"value.h"

class CServerBoard
{
public:
	CServerBoard(int boardSize);
	~CServerBoard(void);

private:
	int** board;
};

