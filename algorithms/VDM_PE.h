/*
 * pop node U -> process and push node (V1, V2, V3, ...) (V1 < V2 < V3 < ...)
 * pop node (U1, ...) -> push node (...) + process node U1 ( -> Vs )
 */

#pragma once
#include <bits/stdc++.h>
#include "SearchingAlgorithm.h"
#include <fmt/core.h>

template<class State>
class VDM_PE: public SearchingAlgorithm<State> {
	private:
		std::map<State, double> f;
		std::set<BigInfo<State>> opening;

		void execute() override {
			State _start = (this -> statement).getSource();
			const State DESTINATION = (this -> statement).getDestination();

			double initH = (this -> statement).heuristic(_start);
			f[_start] = initH; (this -> g)[_start] = 0; (this -> actionTrace)[_start] = "";
			opening.emplace(std::vector<StateInfo<State>>{StateInfo<State>(_start, initH, 0, initH)});

			while (not opening.empty()) {
				this -> UPDATE_SIZE(opening.size());

				auto _it = opening.begin();
				BigInfo<State> _bigNode = *_it; opening.erase(_it);
				StateInfo<State> node = _bigNode;
				if (_bigNode.pop_able()) {
					_bigNode.pop(); opening.insert(_bigNode);
				}

				if (node.state == DESTINATION) {
					this -> FINISH_SEARCHING();
					return;
				}
				if (node.g != (this -> g)[node.state]) 
					continue;
				this -> NEW_ITERATION();
				
				std::vector<StateInfo<State>> all;
				for (auto [action, newState, cost]: (this -> statement).getAdjacent(node.state)) {
					double newG = (node.g) + cost, h = (this -> statement).heuristic(newState),
						   newF = newG + h;
					
					auto it = (this -> g).find(newState);
					if (it != (this -> g).end()) {
						if (it -> second <= newG) continue;

						double oldG = it -> second, oldF = f.find(newState) -> second;
						StateInfo<State> oldNode(newState, oldF, oldG, h);
						// newF = std::min(newF, oldF); might test for optimization
					}
					
					f[newState] = newF; (this -> g)[newState] = newG; (this -> actionTrace)[newState] = action;
					all.push_back(StateInfo<State>(newState, newF, newG, h));
				}
				
				if (not all.empty())
					opening.emplace(all);
			}
		}
	public:
		VDM_PE(Problem<State> statement): SearchingAlgorithm<State>(statement) {
			this -> algoName = fmt::format("VDM's PE");
		}
};