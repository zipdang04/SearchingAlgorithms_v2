#pragma once

#include "SearchingAlgorithm.h"
#include <bits/stdc++.h>

template<class State>
class Dijkstra: public SearchingAlgorithm<State> {
	private:
		std::priority_queue<StateInfo<State>, std::vector<StateInfo<State>>, std::greater<StateInfo<State>>> pq;
	
		void execute() override {
			State _start = (this -> statement).getSource();
			const State DESTINATION = (this -> statement).getDestination();
			
			std::unordered_map<State, int> *g = &(this -> g);
			std::unordered_map<State, std::string> *actionTrace = &(this -> actionTrace);
			
			(*g)[_start] = 0; 
			(*actionTrace)[_start] = "";
			pq.emplace(_start, 0, 0);

			while (not pq.empty()) {
				StateInfo<State> currentInfo = pq.top(); pq.pop();
				if (currentInfo.g != (*g)[currentInfo.state]) continue;
				if (currentInfo.state.isSolved()) {
					this -> FINISH_SEARCHING();
					break;
				}
				std::cerr << currentInfo.state.X() << ' ' << currentInfo.state.Y() << '|';
				std::cerr << currentInfo.state.toString() << ' ' << currentInfo.f << ' ' << currentInfo.g << '|' << (*actionTrace)[currentInfo.state] << '\n';

				for (auto [action, newState]: (this -> statement).getAdjacent(currentInfo.state)) {
					auto it = g -> find(newState);
					if (it != g -> end() and it -> second <= currentInfo.g)	// currently better
						continue;

					int newG = currentInfo.g + 1;
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