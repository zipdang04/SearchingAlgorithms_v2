#include "base/SearchingAlgorithm.h"

template<class State>
class AStarPartialExpansion: public SearchingAlgorithm<State> {	
	private:
		std::set<StateInfo<State>> opening, closed;
		std::map<State, double> f;
		
		void execute() override {
			State _start = (this -> statement).getSource();
			StateInfo<State> _startInfo = this -> buildStateInfo(_start, 0);

			f[_start] = _startInfo.h; (this -> g)[_start] = 0; (this -> actionTrace)[_start] = "";
			opening.emplace(_startInfo);

			while (not opening.empty()) {
				this -> UPDATE_SIZE(opening.size() + closed.size());
				StateInfo<State> node = *opening.begin(); opening.erase(node); 
				if (node.state.isSolved()) {
					this -> FINISH_SEARCHING(node.state);
					return;
				}
				this -> NEW_ITERATION();

				double minClosed = 1e100; bool isMinUpdated = false;
				for (auto [action, newState, cost]: (this -> statement).getAdjacent(node.state)) {
					StateInfo<State> newNode = this -> buildStateInfo(newState, node.g + cost);
					
					auto it = (this -> g).find(newState);
					if (it != (this -> g).end()) {
						if (it -> second <= newNode.g) continue;

						double oldG = it -> second;
						StateInfo<State> oldNode = this -> buildStateInfo(newState, oldG);
						opening.erase(oldNode);
						closed.erase(oldNode);
						// newNode.f = std::min(newNode.f, oldF); might test for optimization
					}
					
					f[newState] = newNode.f; (this -> g)[newState] = newNode.g; (this -> actionTrace)[newState] = action;
					if (newNode.f <= node.f + cBound) 
						opening.emplace(newNode);
					else {
						minClosed = std::min(minClosed, newNode.f),
						isMinUpdated = true;
					}
				}

				if (not isMinUpdated) 
					closed.insert(node);
				else if (minClosed < node.f) {
					node.f = f[node.state] = minClosed;
					opening.insert(node);
				}
			}

		}

	public:
		
		double cBound;
		AStarPartialExpansion(Problem<State> statement, double cBound): SearchingAlgorithm<State>(statement), cBound(cBound) {
			this -> algoName =  "AStar with Partial Expansion with C = " + std::to_string(cBound);
		}
};