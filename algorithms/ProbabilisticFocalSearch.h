#include "SearchingAlgorithm.h"
#include "testlib.h"
#include <fmt/core.h>

template<class State>
class ProbabilisticFocalSearch: public SearchingAlgorithm<State> {
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

		void updateFocal(double oldBound, double newBound) {
			auto it = openList.lower_bound(StateInfo<State>(State(), oldBound, 0, 0));
			while (it != openList.end() and it -> f <= newBound) {
				focalList.insert(*it); it++;
			}
		}
		void execute() override {
			State _start = (this -> statement).getSource();
			const State DESTINATION = (this -> statement).getDestination();

			(this -> g)[_start] = 0; 
			(this -> actionTrace)[_start] = "";
			double initH = (this -> statement).heuristic(_start);
			openList.emplace(_start, initH, 0, initH);
			focalList.emplace(_start, initH, 0, initH);

			while (not openList.empty()) {
				double fMin = openList.begin() -> f;
				StateInfo<State> node = focalList.empty() ? *openList.begin() : (
					(rnd.next(1.0) < probFocal) ? *focalList.begin() : *openList.begin()
				);
				focalList.erase(node); openList.erase(node);
				if (node.state == DESTINATION) {
					this -> FINISH_SEARCHING();
					return;
				}
				this -> NEW_ITERATION();
				
				for (auto [action, newState, cost]: (this -> statement).getAdjacent(node.state)) {
					double newG = node.g + cost, h = (this -> statement).heuristic(newState);
					double newF = newG + h;

					auto itG = (this -> g).find(newState);
					if (itG != (this -> g).end()) {
						double oldG = itG -> second;
						if (oldG <= newG) continue;

						openList.erase(StateInfo<State>(newState, oldG + h, oldG, h));
						focalList.erase(StateInfo<State>(newState, oldG + h, oldG, h));
					}

					(this -> g)[newState] = newG;
					(this -> actionTrace)[newState] = action;

					StateInfo<State> newNode(newState, newF, newG, h);
					openList.insert(newNode);
					if (newNode.f <= round(fMin * eps)) 
						focalList.insert(newNode);
				}

				if (not openList.empty()) {
					int fHead = openList.begin() -> f;
					if (fHead > fMin)
						updateFocal(fMin * eps, fHead * eps);
				}
			}
		}
	public:
		double eps, probFocal;
		ProbabilisticFocalSearch(Problem<State> statement, double eps, double probFocal): SearchingAlgorithm<State>(statement), eps(eps), probFocal(probFocal) {
			this -> algoName = fmt::format("Probabilistic Focal Search with eps = {}; pFocal = {}", eps, probFocal);
		}
};