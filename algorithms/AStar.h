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
			
			std::unordered_map<State, double> *g = &(this -> g);
			std::unordered_map<State, std::string> *actionTrace = &(this -> actionTrace);
			
			(*g)[_start] = 0; 
			(*actionTrace)[_start] = "";
			double initH = (this -> statement).heuristic(_start);
			pq.emplace(_start, initH, 0, initH);

			while (not pq.empty()) {
				StateInfo<State> currentInfo = pq.top(); pq.pop();
				if (currentInfo.g != (*g)[currentInfo.state]) continue;
				// std::cerr << currentInfo.state.X() << ' ' << currentInfo.state.Y() << '|';
				// std::cerr << currentInfo.state.toString() << ' ' << currentInfo.f << ' ' << currentInfo.g << '|' << (*actionTrace)[currentInfo.state] << '\n';
				if (currentInfo.state.isSolved()) {
					this -> FINISH_SEARCHING();
					break;
				}

				for (auto [action, newState, cost]: (this -> statement).getAdjacent(currentInfo.state)) {
					auto it = g -> find(newState);
					if (it != g -> end() and it -> second <= currentInfo.g)	// currently better
						continue;

					double newH = (this -> statement).heuristic(newState);
					double newG = currentInfo.g + cost;
					(*g)[newState] = newG;
					(*actionTrace)[newState] = action;
					pq.emplace(newState, newG + newH, newG, newH);

					// if (newState == DESTINATION) {
					// 	this -> FINISH_SEARCHING();
					// 	return;
					// }
				}
			}
		}
	public:
		AStar(Problem<State> statement): SearchingAlgorithm<State>(statement) {
			this -> algoName =  "AStar";
		}
};