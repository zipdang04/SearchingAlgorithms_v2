#pragma once
#include <bits/stdc++.h>
#include "../base/exceptions.h"

template<typename State>
struct Transition {
	std::string action;
	State state;
	double cost;
};
template<typename State>
class Problem {
	protected:
		State SOURCE, DESTINATION;
	public:
		Problem(State SOURCE, State DESTINATION) {
			this -> SOURCE = SOURCE;
			this -> DESTINATION = DESTINATION;
		}
		State getSource() { return SOURCE; }
		State getDestination() { return DESTINATION; }
		
		virtual double heuristic(State state) {
			throw NotImplementedException("no heuristics implemented");
		}
		virtual std::vector<Transition<State>> getAdjacent(State state) {
			throw NotImplementedException("no adjacent-finding functions implemented");
		}
};