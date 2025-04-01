#include "SearchingAlgorithm.h"

template<class State>
class AStarPartialExpansion: public SearchingAlgorithm<State> {	
	private:

	public:
		AStar(Problem<State> statement): SearchingAlgorithm<State>(statement) {
			this -> algoName =  "AStar with Partial Expansion";
		}
};