#include "SearchingAlgorithm.h"

template<class State>
class AStarPartialExpansion: public SearchingAlgorithm<State> {	
	private:
		std::set<StateInfo<State>> opening, closed;
		std::map<State, double> f;
		
		void execute() override {
			State _start = (this -> statement).getSource();
			const State DESTINATION = (this -> statement).getDestination();

			double initH = (this -> statement).heuristic(_start);
			f[_start] = initH; (this -> g)[_start] = 0; (this -> actionTrace)[_start] = "";
			opening.emplace(_start, initH, 0, initH);

			while (not opening.empty()) {
				this -> UPDATE_SIZE(opening.size() + closed.size());
				StateInfo<State> node = *opening.begin(); opening.erase(node); 
				if (node.state.isSolved()) {
					this -> FINISH_SEARCHING();
					return;
				}
				this -> NEW_ITERATION();

				double minClosed = 1e100; bool isMinUpdated = false;
				for (auto [action, newState, cost]: (this -> statement).getAdjacent(node.state)) {
					double newG = (node.g) + cost, h = (this -> statement).heuristic(newState),
						   newF = newG + h;
					
					auto it = (this -> g).find(newState);
					if (it != (this -> g).end()) {
						if (it -> second <= newG) continue;

						double oldG = it -> second, oldF = f.find(newState) -> second;
						StateInfo<State> oldNode(newState, oldF, oldG, h);
						opening.erase(oldNode);
						closed.erase(oldNode);
						// newF = std::min(newF, oldF); might test for optimization
					}
					
					f[newState] = newF; (this -> g)[newState] = newG; (this -> actionTrace)[newState] = action;
					if (newF <= node.f + cBound) 
						opening.emplace(newState, newF, newG, h);
					else {
						minClosed = std::min(minClosed, newF),
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