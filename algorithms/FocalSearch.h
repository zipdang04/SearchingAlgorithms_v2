#pragma once

#include "base/SearchingAlgorithm.h"

template<class State>
class FocalSearch: public SearchingAlgorithm<State> {
	private:
		struct CompareF {
			bool operator() (StateInfo<State> a, StateInfo<State> b) const {
				if (a.f != b.f) return a.f < b.f;
				if (a.g != b.g) return a.g < b.g;
				if (a.h != b.h) return a.h < b.h;
				if (a.hFocal != b.hFocal) return a.hFocal < b.hFocal;
				return a.state < b.state;
			}
		};
		struct CompareH {
			bool operator() (StateInfo<State> a, StateInfo<State> b) const {
				if (a.hFocal != b.hFocal) return a.hFocal < b.hFocal;
				if (a.h != b.h) return a.h < b.h;
				if (a.g != b.g) return a.g < b.g;
				if (a.f != b.f) return a.f < b.f;
				return a.state < b.state;
			}
		};
		std::set<StateInfo<State>, CompareF> openList;
		std::set<StateInfo<State>, CompareH> focalList;

		void updateFocal(double oldBound, double newBound) {
			auto it = openList.lower_bound(StateInfo<State>(State(), oldBound, 0, 0, 0));
			while (it != openList.end() and it -> f <= newBound) {
				focalList.insert(*it); it++;
			}
		}
		void execute() override {
			State _start = (this -> statement).getSource();
			StateInfo<State> _startInfo = this -> buildStateInfo(_start, 0);

			(this -> g)[_start] = 0; 
			(this -> actionTrace)[_start] = "";
			openList.emplace(_startInfo);
			focalList.emplace(_startInfo);

			while (not openList.empty()) {
				this -> UPDATE_SIZE(openList.size() + focalList.size());

				double fMin = openList.begin() -> f;
				
				StateInfo<State> node = (not focalList.empty()) ? *focalList.begin() : *openList.begin();
				focalList.erase(node); openList.erase(node);
				if (node.state.isSolved()) {
					this -> FINISH_SEARCHING(node.state);
					return;
				}
				if (this -> ITERATION_CHECK() == true) return;
				
				for (auto [action, newState, cost]: (this -> statement).getAdjacent(node.state)) {
					StateInfo<State> newNode = this -> buildStateInfo(newState, node.g + cost);
					// double newG = node.g + cost, h = (this -> statement).heuristic(newState);
					// double newF = newG + h;

					auto itG = (this -> g).find(newState);
					if (itG != (this -> g).end()) {
						double oldG = itG -> second;
						if (oldG <= newNode.g) continue;

						StateInfo<State> oldNode = this -> buildStateInfo(newState, oldG);
						openList.erase(oldNode);
						focalList.erase(oldNode);
					}

					(this -> g)[newState] = newNode.g;
					(this -> actionTrace)[newState] = action;

					openList.insert(newNode);
					if (newNode.f <= fMin * eps) 
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
		double eps;
		FocalSearch(Problem<State> statement, double eps): SearchingAlgorithm<State>(statement), eps(eps) {
			this -> algoName = "Focal Search with eps = " + std::to_string(eps);
		}
};