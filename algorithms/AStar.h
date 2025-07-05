#pragma once

#include "base/SearchingAlgorithm.h"
#include <bits/stdc++.h>

template<class State>
class AStar: public SearchingAlgorithm<State> {
	private:
		std::priority_queue<StateInfo<State>, std::vector<StateInfo<State>>, std::greater<StateInfo<State>>> pq;
	
		void execute() override {
			State _start = (this -> statement).getSource();
			StateInfo<State> _startInfo = this -> buildStateInfo(_start, 0);

			(this -> g)[_start] = 0; 
			(this -> actionTrace)[_start] = "";
			pq.emplace(_startInfo);

			while (not pq.empty()) {
				StateInfo<State> currentInfo = pq.top(); pq.pop();
				if (currentInfo.g != (this -> g)[currentInfo.state]) continue;
				if (currentInfo.state.isSolved()) {
					this -> FINISH_SEARCHING(currentInfo.state);
					break;
				}
				if (this -> ITERATION_CHECK() == true) return;

				for (auto [action, newState, cost]: (this -> statement).getAdjacent(currentInfo.state)) {
					StateInfo<State> newNode = this -> buildStateInfo(newState, currentInfo.g + cost);
					
					auto it = (this -> g).find(newState);
					if (it != (this -> g).end() and it -> second <= newNode.g)	// currently better
						continue;
					
					(this -> g)[newState] = newNode.g;
					(this -> actionTrace)[newState] = action;
					pq.emplace(newNode);
				}
			}
		}
	public:
		AStar(Problem<State> statement): SearchingAlgorithm<State>(statement) {
			this -> algoName =  "AStar";
		}
};