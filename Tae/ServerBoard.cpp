#include "ServerBoard.h"


CServerBoard::CServerBoard(int boardSize) {

	board = new int* [boardSize];

	for(int i = 0; i < boardSize; i++) {

		board[i] = new int [boardSize];
	}

	memset(board, 0, sizeof(int) * boardSize * boardSize);
}

CServerBoard::~CServerBoard(void) {

}
