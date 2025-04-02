#include "SearchingAlgorithm.h"

template<class State>
class AStarPartialExpansion: public SearchingAlgorithm<State> {	
	private:
		std::set<StateInfo<State>> opening, closed;
		
		void execute() override {
			State _start = (this -> statement).getSource();
			const State DESTINATION = (this -> statement).getDestination();

			std::unordered_map<State, double> *g = &(this -> g);
			std::unordered_map<State, std::string> *actionTrace = &(this -> actionTrace);

			(*g)[_start] = 0; 
			(*actionTrace)[_start] = "";
			double initH = (this -> statement).heuristic(_start);
			opening.emplace(_start, initH, 0, initH);

			while (not opening.empty()) {
				auto _it = opening.begin(); opening.erase(_it);
				if (_it -> state == DESTINATION) {
					this -> FINISH_SEARCHING();
					return;
				}

				double minClosed = 1e100;
				for (auto [action, newBoard, cost]: (this -> statement).getAdjacent(_it -> state)) {
					
				}
			}
		}

	public:
		
		double cBound;
		AStarPartialExpansion(Problem<State> statement, double cBound): SearchingAlgorithm<State>(statement), cBound(cBound) {
			this -> algoName =  "AStar with Partial Expansion";
		}
};