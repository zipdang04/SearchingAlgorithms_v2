#include "SearchingAlgorithm.h"

template<class State>
class FocalSearch: public SearchingAlgorithm<State> {
	private:
		struct CompareF {
			static bool operator() (StateInfo<State> a, StateInfo<State> b) {
				if (a.f != b.f) return a.f < b.f;
				if (a.g != b.g) return a.g < b.g;
				if (a.h != b.h) return a.h < b.h;
				return a.state < b.state;
			}
		};
		struct CompareH {
			static bool operator() (StateInfo<State> a, StateInfo<State> b) {
				if (a.h != b.h) return a.h < b.h;
				if (a.g != b.g) return a.g < b.g;
				if (a.f != b.f) return a.f < b.f;
				return a.state < b.state;
			}
		};
		std::set<StateInfo<State>, CompareF> openList;
		std::set<StateInfo<State>, CompareH> focalList;

		void updateFocal(int lastFMin) {
			lastFMin = round(lastFMin * eps);
			auto it = openList.lower_bound(StateInfo<State>(State(), 0, 0, 0));
			while (it != openList.end() and it -> f <= lastFMin) {
				focalList.insert(*it); it++;
			}
		}
		void execute() override {
			State _start = (this -> statement).getSource();
			const State DESTINATION = (this -> statement).getDestination();

			std::unordered_map<State, int> *g = &(this -> g);
			std::unordered_map<State, std::string> *actionTrace = &(this -> actionTrace);

			(*g)[_start] = 0; 
			(*actionTrace)[_start] = "";
			int initH = (this -> statement).heuristic(_start);
			openList.emplace(_start, initH, 0, initH);
			focalList.emplace(_start, initH, 0, initH);

			while (not openList.empty()) {
				int fMin = openList.begin() -> f;
				
				StateInfo<State> node = (not focalList.empty()) ? *focalList.begin() : *openList.begin();
				focalList.erase(node); openList.erase(node);
				if (node.state == DESTINATION) {
					this -> FINISH_SEARCHING();
					return;
				}
				
				for (auto [action, newState]: (this -> statement).getAdjacent(node.state)) {
					int newG = node.g + 1, h = (this -> statement).heuristic(newState);
					int newF = newG + h;

					auto itG = g -> find(newState);
					if (itG != g -> end()) {
						int oldG = itG -> second;
						if (oldG <= newG) continue;

						openList.erase(StateInfo<State>(newState, oldG + h, oldG, h));
						focalList.erase(StateInfo<State>(newState, oldG + h, oldG, h));
					}

					(*g)[newState] = newG;
					(*actionTrace)[newState] = action;

					StateInfo<State> newNode(newState, newF, newG, h);
					openList.insert(newNode);
					if (newNode.f <= round(fMin * eps)) 
						focalList.insert(newNode);
				}

				updateFocal(fMin);
			}
		}
	public:
		double eps;
		FocalSearch(Problem<State> statement, double eps): SearchingAlgorithm<State>(statement), eps(eps) {
			this -> algoName = "Focal Search with eps = " + std::to_string(eps);
		}
};