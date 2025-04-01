#pragma once

#include "heuristics.h"

template<>
int Problem<Board>::heuristic(Board board) {
	return Heuristic::ManhattanDistance(board);
}
template<>
std::vector<std::pair<std::string, Board>> Problem<Board>::getAdjacent(Board board) {
	std::vector<BoardMove> validMoves = BoardMove::ALL();
	std::vector<std::pair<std::string, Board>> v;
	for (BoardMove move: validMoves) {
		try {
			v.emplace_back(move.c, board.move(move));
		} catch (InvalidMoveException ex) {}
	}
	return v;
}