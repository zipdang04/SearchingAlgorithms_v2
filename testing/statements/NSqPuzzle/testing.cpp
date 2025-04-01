#include "testlib.h"
#include <bits/stdc++.h>
#include "../../../statements/NSqPuzzle/Board.h"

int SIZE;

void TestInit() {
	Board board = Board();
	for (int i = 0; i < Board::SIZE; i++)
		ensure(board[i] == i);
	ensure(board.X() == 0 and board.Y() == 0);
	ensure(board.isSolved());
}

void TestMove() {
	Board board = Board();
	try {
		board = board.move(BoardMove::GET_L());
		quitf(_wa, "Should not move left");
	} catch (InvalidMoveException ex) {}
	try {
		board = board.move(BoardMove::GET_U());
		quitf(_wa, "Should not move up");
	} catch (InvalidMoveException ex) {}

	int posX = 0, posY = 0;
	for (int i = 0; i + 1 < SIZE; i++) {
		board = board.move(BoardMove::GET_R()); posY++;
		// std::cerr << board.toString() << ' ' << board.X() << ' ' << board.Y() << ' ' << posX << ' ' << posY << '\n';
		ensure(board.X() == posX and board.Y() == posY);
		board = board.move(BoardMove::GET_D()); posX++;
		ensure(board.X() == posX and board.Y() == posY);
	}

	try {
		board = board.move(BoardMove::GET_R());
		quitf(_wa, "Should not move right");
	} catch (InvalidMoveException ex) {}
	try {
		board = board.move(BoardMove::GET_D());
		quitf(_wa, "Should not move down");
	} catch (InvalidMoveException ex) {}

	for (int i = 0; i + 1 < SIZE; i++) {
		board = board.move(BoardMove::GET_L()); posY--;
		// std::cerr << board.toString() << ' ' << board.X() << ' ' << board.Y() << ' ' << posX << ' ' << posY << '\n';
		ensure(board.X() == posX and board.Y() == posY);
		board = board.move(BoardMove::GET_U()); posX--;
		ensure(board.X() == posX and board.Y() == posY);
	}
}

int main(int argc, char** argv) {
	registerGen(argc, argv, 1);
	SIZE = opt<int>("size");
	Board _INIT(SIZE);

	TestInit();
	TestMove();
}