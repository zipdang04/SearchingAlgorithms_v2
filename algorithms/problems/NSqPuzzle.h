#pragma once

#include "../base/SearchingAlgorithm.h"
#include "../../statements/NSqPuzzle/Problem.h"
#include <fmt/core.h>
template<>
std::vector<std::string> SearchingAlgorithm<Board>::getTrace() {
	if (not FINISHED) return {};

	Board board = statement.getDestination();
	std::vector<std::string> actions;

	while (board != statement.getSource()) {
		std::string action = actionTrace[board];
		// std::cerr << board.toString() << ' ' << action << '\n';
		actions.push_back(action);
		if (action == "L") board = board.move(BoardMove::GET_R());
		else if (action == "R") board = board.move(BoardMove::GET_L());
		else if (action == "U") board = board.move(BoardMove::GET_D());
		else if (action == "D") board = board.move(BoardMove::GET_U());
		else throw InvalidTraceException(fmt::format("There is an invalid move ({}) when tracing from {}", action, board.toString()));
	}

	std::reverse(actions.begin(), actions.end());
	return actions;
}