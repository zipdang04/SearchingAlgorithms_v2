#pragma once

#include "../Problem.h"

class BoardMove {
	private:
		BoardMove(std::string c, int diffX, int diffY): c(c), diffX(diffX), diffY(diffY) {}
	public:
		std::string c; int diffX, diffY;

		static BoardMove GET_L() { return BoardMove("L", 0, -1); }
		static BoardMove GET_R() { return BoardMove("R", 0, +1); } 
		static BoardMove GET_U() { return BoardMove("U", -1, 0); } 
		static BoardMove GET_D() { return BoardMove("D", +1, 0); }
		static std::vector<BoardMove> ALL() {
			static const std::vector<BoardMove> ALL = {
				GET_L(), GET_U(), GET_R(), GET_D()
			};
			return ALL;
		}
		
		std::pair<int, int> from(int x, int y) {
			return {x + diffX, y + diffY};
		}
};

class Board {
	private:
		static bool DEBUG_MODE;

		int blankX, blankY;
		std::vector<int> board;

		void _selfMove(BoardMove move);
		static inline int getIndex(int row, int col) { return row * SIZE + col; }
	public:
		static void DEBUG() { DEBUG_MODE = true; }

		static const int BLANK = 0, NaN = -1;
		static int SIZE, MAX_VALUE;
		static inline void changeSize(int size);
		
		Board(std::string tmp) {if (tmp != "NOT_INIT") throw NotImplementedException("Not initialized yet");}
		Board();
		Board(int size);
		Board(std::vector<int> board, int blankX, int blankY);

		inline int operator[] (int idx) const;
		inline int get(int row, int col) const;
		inline bool operator != (Board board) const;
		inline bool operator == (Board board) const;
		inline bool operator < (Board board) const;
		inline bool operator > (Board board) const;

		inline int X() { return blankX; }
		inline int Y() { return blankY; }
		inline bool isSolved();
		inline bool isSolvable();

		inline Board move(BoardMove move);
		std::string toString() const;
};

class InvalidMoveException: public Exception {
	const std::string PREFIX = "InvalidMoveException";
	Board board;
	public:
		InvalidMoveException(Board board, BoardMove move): Exception(PREFIX, "") {
			message = std::format("Tried to move {} at board {}", move.c, board.toString());
		}
};
class InvalidBoardException: public Exception {
	const std::string PREFIX = "InvalidMoveException";
	public:
		InvalidBoardException(std::string message): Exception(PREFIX, message) {}
};


template<>
inline int Problem<Board>::heuristic(Board);

template<>
std::vector<std::pair<std::string, Board>> Problem<Board>::getAdjacent(Board);

//========================================================================================================================================================================

int Board::SIZE = 0;
int Board::MAX_VALUE = 0;

void Board::changeSize(int size) {
	if (SIZE != 0)
		throw InvalidBoardException("Board SIZE already defined");
	SIZE = size; MAX_VALUE = size * size;
}

Board::Board() {
	if (SIZE == 0)
		throw InvalidBoardException("Board SIZE not yet defined");
	board = std::vector<int>(MAX_VALUE);
	blankX = 0, blankY = 0;
	std::iota(board.begin(), board.end(), 0);
}
Board::Board(int size) {
	if (size == 0)
		throw InvalidBoardException("Init size must be different than 0");
	changeSize(size);

	blankX = 0, blankY = 0;
	board = std::vector<int>(MAX_VALUE);
	std::iota(board.begin(), board.end(), 0);
}
Board::Board(std::vector<int> board, int blankX = 0, int blankY = 0) {
	if (SIZE == 0)
		throw InvalidBoardException("Board SIZE not yet defined");
	if (MAX_VALUE != (int)board.size())
		throw InvalidBoardException("The board inputed did not match the required size");
	this->board = board;
	if (this->get(blankX, blankY) != BLANK) 
		for (int x = 0; x < SIZE; x++) for (int y = 0; y < SIZE; y++) {
			if (this->get(x, y) == BLANK) {
				blankX = x, blankY = y;
				break;
			}
		}
	
	this -> blankX = blankX;
	this -> blankY = blankY;
}

int Board::operator[] (int idx) const { return board[idx]; }
int Board::get(int row, int col) const { return board[getIndex(row, col)]; }
bool Board::operator < (Board board) const {
	for (int i = 0; i < MAX_VALUE; i++)
		if (this->board[i] != board[i])
			return this->board[i] < board[i];
	return false;
}
bool Board::operator > (Board board) const {
	for (int i = 0; i < MAX_VALUE; i++)
		if (this->board[i] != board[i])
			return this->board[i] > board[i];
	return false;
}
bool Board::operator == (Board board) const {
	for (int i = 0; i < MAX_VALUE; i++)
		if (this->board[i] != board[i])
			return false;
	return true;
}
bool Board::operator != (Board board) const {
	return not (*this == board);
}


void Board::_selfMove(BoardMove move) {
	auto [newX, newY] = move.from(blankX, blankY);
	if (newX < 0 or newY < 0 or newX >= SIZE or newY >= SIZE)
		throw InvalidMoveException(*this, move);
	
	std::swap(board[getIndex(blankX, blankY)], board[getIndex(newX, newY)]);
	blankX = newX, blankY = newY;
}

Board Board::move(BoardMove move) {
	Board board = *this;
	board._selfMove(move);
	return board;
}
std::string Board::toString() const {
	std::string s = "[";
	int ptr = 0;
	for (int row = 0; row < SIZE; row++) {
		std::string ending = " "; 
		ending +=  char((row == SIZE - 1) ? ']' : ';');
		for (int col = 0; col < SIZE; col++)
			s += std::to_string(board[ptr++]) + ending[col == SIZE - 1];
	}
	return s;
}

bool Board::isSolved() {
	for (int i = 0; i < MAX_VALUE; i++)
		if (board[i] != i) return false;
	return true;
}

bool Board::isSolvable() {
	return true;
}

template<>
struct std::hash<Board> {
	std::size_t operator()(const Board &p) const
    {
        unsigned long long num = 0;
		for (int i = 0; i < Board::MAX_VALUE; i++)
			num = (num << 5) | p[i];
		return num;
    }
};