#pragma once
#include "Board.h"

namespace Heuristic {
	inline int ManhattanDistance(Board board) {
		int ptr = 0, answer = 0;
		for (int row = 0; row < Board::SIZE; row++)
		for (int col = 0; col < Board::SIZE; col++, ptr++){
			int value = board.get(row, col);
			int vRow = value / Board::SIZE, vCol = value - Board::SIZE * vRow;
			answer += (value > 0) * (std::abs(row - vRow) + std::abs(col - vCol));
		}
		return answer;
	}
	
}