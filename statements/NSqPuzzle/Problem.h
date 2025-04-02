#pragma once

#include "heuristics.h"

template<>
double Problem<Board>::heuristic(Board board) {
	return Heuristic::ManhattanDistance(board);
}
template<>
std::vector<Transition<Board>> Problem<Board>::getAdjacent(Board board) {
	std::vector<BoardMove> validMoves = BoardMove::ALL();
	std::vector<Transition<Board>> v;
	for (BoardMove move: validMoves) {
		try {
			v.emplace_back(move.c, board.move(move), 1);
		} catch (InvalidMoveException ex) {}
	}
	return v;
}