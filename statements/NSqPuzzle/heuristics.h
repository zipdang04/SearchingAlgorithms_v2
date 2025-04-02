#pragma once
#include "Board.h"

namespace Heuristic {
	inline double ManhattanDistance(Board board) {
		int ptr = 0, answer = 0;
		for (int row = 0; row < Board::SIZE; row++)
		for (int col = 0; col < Board::SIZE; col++, ptr++){
			int value = board.get(row, col);
			int vRow = value / Board::SIZE, vCol = value - Board::SIZE * vRow;
			answer += (value > 0) * (std::abs(row - vRow) + std::abs(col - vCol));
		}
		return answer;
	}
	inline double LinearConflictDistance(Board board) {
		static const auto getRowCol = [](int index) -> std::pair<int, int> {
			return {index / Board::SIZE, index % Board::SIZE};
		};
		static const auto isConflicted = [](int num1, int num2, bool isRow, int designated) -> bool {
			auto [r1, c1] = getRowCol(num1);
			auto [r2, c2] = getRowCol(num2);
			return isRow ? (
				r1 == designated 
				and r2 == designated
				and c1 > c2
			) : (
				c1 == designated
				and c2 == designated
				and r1 > r2
			);
		};

		int answer = 0;
		for (int x = 0; x < Board::SIZE; x++)
		for (int y1 = 0; y1 < Board::SIZE; y1++)
		for (int y2 = y1 + 1; y2 < Board::SIZE; y2++) {
			answer += 2 * isConflicted(
				board.get(x, y1),
				board.get(x, y2),
				true, x
			);
			answer += 2 * isConflicted(
				board.get(y1, x),
				board.get(y2, x),
				false, x
			);
		}

		return answer + ManhattanDistance(board);
	}
	
}