#pragma once

#include "SearchingAlgorithm.h"
#include <bits/stdc++.h>

template<class State>
class AStar: public SearchingAlgorithm<State> {
	private:
		std::priority_queue<StateInfo<State>, std::vector<StateInfo<State>>, std::greater<StateInfo<State>>> pq;
	
		void execute() override {
			State _start = (this -> statement).getSource();
			const State DESTINATION = (this -> statement).getDestination();
			
			std::map<State, double> *g = &(this -> g);
			std::map<State, std::string> *actionTrace = &(this -> actionTrace);
			
			(*g)[_start] = 0; 
			(*actionTrace)[_start] = "";
			double initH = (this -> statement).heuristic(_start);
			pq.emplace(_start, initH, 0, initH);

			while (not pq.empty()) {
				StateInfo<State> currentInfo = pq.top(); pq.pop();
				if (currentInfo.g != (*g)[currentInfo.state]) continue;
				if (currentInfo.state.isSolved()) {
					this -> FINISH_SEARCHING();
					break;
				}
				this -> NEW_ITERATION();

				for (auto [action, newState, cost]: (this -> statement).getAdjacent(currentInfo.state)) {
					double newG = currentInfo.g + cost, h = (this -> statement).heuristic(newState);
					double newF = newG + h;

					auto it = g -> find(newState);
					if (it != g -> end() and it -> second <= newG)	// currently better
						continue;

					(*g)[newState] = newG;
					(*actionTrace)[newState] = action;
					pq.emplace(newState, newF, newG, h);
				}
			}
		}
	public:
		AStar(Problem<State> statement): SearchingAlgorithm<State>(statement) {
			this -> algoName =  "AStar";
		}
};