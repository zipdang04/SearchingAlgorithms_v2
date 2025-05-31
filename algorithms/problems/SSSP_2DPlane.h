#pragma once

#include "../SearchingAlgorithm.h"
#include "../../statements/SSSP_2DPlane/Problem.h"
#include <fmt/core.h>
template<>
std::vector<std::string> SearchingAlgorithm<Node>::getTrace() {
	if (not FINISHED) return {};

	Node node = statement.getDestination();
	std::vector<std::string> actions;

	while (node != statement.getSource()) {
		std::string action = actionTrace[node];
		// std::cerr << board.toString() << ' ' << action << '\n';
		actions.push_back(action);
		Node prev = Node(std::stoi(action));
		node = prev;
	}

	std::reverse(actions.begin(), actions.end());
	return actions;
}