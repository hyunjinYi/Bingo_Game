#include "ClientBoard.h"

CClientBoard::CClientBoard(int size, int number){

	boardSize = size;
	goalBingo = number;
	curBingo = 0;
	userState = false;
	curNum = 0;
	usedList = new int [size * size];
	memset(usedList, 0, sizeof(int) * size * size);
	listIdx = -1;

	board = new int* [boardSize];
	markBoard = new int* [boardSize];
	positionNum = new int* [boardSize * boardSize + 1];

	for(int i = 0; i < boardSize; i++){

		board[i] = new int[boardSize];
		markBoard[i] = new int[boardSize];
		memset(board[i], 0, sizeof(int) * boardSize);
		memset(markBoard[i], 0, sizeof(int) * boardSize);
	}

	for(int i = 0; i < (boardSize * boardSize) + 1; i++)
		positionNum[i] = new int [2];

	positionNum[0][0] = 0;
	positionNum[0][1] = 0;

	checkRow = new int[boardSize + 1];
	checkCol = new int[boardSize + 1];

	for(int i = 0; i < boardSize + 1; i++){

		memset(checkRow, 0, sizeof(int) * (boardSize + 1));
		memset(checkCol, 0, sizeof(int) * (boardSize + 1));
	}
	shuffleNumber();
}

CClientBoard::~CClientBoard(void) {
}

void CClientBoard::shuffleNumber(){

	int temp;
	int numberSize = boardSize * boardSize;
	flag = new int[numberSize];

	srand((unsigned int)time(NULL));
	Sleep(500);

	for (int i = 0; i < numberSize; i++)
		memset(flag, 0, sizeof(int) * numberSize);

	for (int i = 0; i < boardSize; i++){
		for (int j = 0; j < boardSize; j++){

			temp = rand() % numberSize + 1;

			while (flag[temp - 1] != 0) 
				temp = rand() % numberSize + 1;

			flag[temp - 1] = 1;
			board[i][j] = temp;
			positionNum[temp][0] = i;
			positionNum[temp][1] = j;
		}
	}
}

void CClientBoard::distributeNumber(){

	int row = positionNum[curNum][0];
	int col = positionNum[curNum][1];
	markBoard[row][col] = 1;

	if (++checkRow[row + 1] == boardSize)
		curBingo++;

	if (++checkCol[col + 1] == boardSize)
		curBingo++;

	if (row == col){
		if (++checkRow[0] == boardSize)
			curBingo++;
	}

	if (row + col == boardSize - 1){
		if (++checkCol[0] == boardSize)
			curBingo++;
	}

	for (int i = 0; i < boardSize + 1; i++)
		cout << checkRow[i] << " ";

	cout << endl;

	for (int i = 0; i < boardSize + 1; i++)
		cout<<checkCol[i]<<" ";

	cout<<endl;
	cout<<curBingo<<" \n";
}

void CClientBoard::printInfo() {

	system("cls");
	cout << "Goal Bingo Count : " << goalBingo << endl;
	cout << "Current Bingo Count : " << curBingo << endl;
	cout << "==== Used Number List ==== " << endl;

	if(listIdx == -1) 
		cout << "Nobody called a number yet" << endl;

	for(int i = 0; i <= listIdx; i++) {

		cout << " [" << usedList[i] << "] ";

		if((i + 1) % 10 == 0)
			cout << endl;
	}

	cout << endl;

	cout << "==== Your Bingo Board ====" << endl; 

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {

			cout<<"[ ";
			cout.width(2);
			cout.fill(' ');

			if(listIdx == -1)
				cout<<right<<board[i][j];

			for(int k = 0; k <= listIdx; k++) {

				if(board[i][j] == usedList[k]) {

					cout<<right<<"X";
					break;
				}

				if(k == listIdx)
					cout<<right<<board[i][j];
			}
			cout<<" ]";
		}
		cout<<endl;
	}
}

void CClientBoard::setCurNumber(int number){

	curNum = number;
	usedList[++listIdx] = number;
}

bool CClientBoard::checkRepetition(int number){

	if(listIdx == - 1)
		return true;

	for(int i = 0; i <= listIdx; i++){
		if(usedList[i] == number){

			cout << "This number has already been called" << endl;
			return false;
		}
	}
	return true;
}

bool CClientBoard::checkBingo(){

	if(curBingo >= goalBingo)
		return true;
	else
		return false;
}