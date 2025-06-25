#pragma once

#include "heuristics.h"
#include <bits/stdc++.h>

template<>
double Problem<Perm>::heuristic(Perm perm) {
	return Heuristic::GapHeuristic(perm);
}
template<>
std::vector<Transition<Perm>> Problem<Perm>::getAdjacent(Perm board) {
	std::vector<Transition<Perm>> all;
	for (int k = 2; k <= Perm::SIZE; k++) {
		Perm perm = board.move(k);
		all.push_back(Transition<Perm>(
			std::to_string(k), perm, 1
		));
	}
	return all;
}