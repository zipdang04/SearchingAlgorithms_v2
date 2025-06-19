#pragma once

#include "base/SearchingAlgorithm.h"
#include <bits/stdc++.h>

template<class State>
class Dijkstra: public SearchingAlgorithm<State> {
	private:
		std::priority_queue<StateInfo<State>, std::vector<StateInfo<State>>, std::greater<StateInfo<State>>> pq;
	
		void execute() override {
			State _start = (this -> statement).getSource();
			
			std::unordered_map<State, double> *g = &(this -> g);
			std::unordered_map<State, std::string> *actionTrace = &(this -> actionTrace);
			
			StateInfo<State> _startInfo = this -> buildStateInfo(_start, 0);
			(*g)[_start] = 0; 
			(*actionTrace)[_start] = "";
			pq.emplace(_startInfo);

			while (not pq.empty()) {
				StateInfo<State> currentInfo = pq.top(); pq.pop();
				if (currentInfo.g != (*g)[currentInfo.state]) continue;
				if (currentInfo.state.isSolved()) {
					this -> FINISH_SEARCHING(currentInfo.state);
					break;
				}
				this -> NEW_ITERATION();

				for (auto [action, newState, cost]: (this -> statement).getAdjacent(currentInfo.state)) {
					auto it = g -> find(newState);
					if (it != g -> end() and it -> second <= currentInfo.g)	// currently better
						continue;

					double newG = currentInfo.g + cost;
					(*g)[newState] = newG;
					(*actionTrace)[newState] = action;
					pq.emplace(newState, newG, newG, 0);
				}
			}
		}
	public:
		Dijkstra(Problem<State> statement): SearchingAlgorithm<State>(statement) {
			this -> algoName =  "Dijkstra";
		}
};