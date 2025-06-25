#pragma once

#include "../base/SearchingAlgorithm.h"
#include "../../statements/Pancake/Problem.h"
#include <fmt/core.h>
template<>
std::vector<std::string> SearchingAlgorithm<Perm>::getTrace() {
	if (not FINISHED) return {};

	Perm node = ansState;
	std::vector<std::string> actions;

	while (node != statement.getSource()) {
		std::string _action = actionTrace[node];
		actions.push_back(_action);
		int k = atoi(_action.c_str());
		node = node.move(k);
	}

	std::reverse(actions.begin(), actions.end());
	return actions;
}

template<>
inline StateInfo<Perm> SearchingAlgorithm<Perm>::buildStateInfo(Perm state, double g) {
	double h = statement.heuristic(state);
	double f = g + h;
	double hFocal = h;// (Perm::SIZE - state.size()) * 1000000 + h;//g + 1.05 * h;
	return StateInfo<Perm>(state, f, g, h, hFocal);
}

