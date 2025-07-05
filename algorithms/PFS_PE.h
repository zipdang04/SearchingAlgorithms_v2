#pragma once
#include <bits/stdc++.h>
#include "base/SearchingAlgorithm.h"
#include <fmt/core.h>
template<class State>
class ProbabilisticFocalSearch_PartialExpansion: public SearchingAlgorithm<State> {
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
		std::map<State, double> f;
		std::set<StateInfo<State>, CompareF> openList;
		std::set<StateInfo<State>, CompareH> focalList;
		std::set<StateInfo<State>, CompareF> closedList;

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
			(this -> f)[_start] = _startInfo.h;
			openList.emplace(_startInfo);
			focalList.emplace(_startInfo);

			while (not openList.empty()) {
				this -> UPDATE_SIZE(openList.size() + closedList.size() + focalList.size());

				double fMin = openList.begin() -> f;
				StateInfo<State> node = focalList.empty() ? *openList.begin() : (
					(rnd.next(1.0) < probFocal) ? *focalList.begin() : *openList.begin()
				);
				focalList.erase(node); openList.erase(node);
				if (node.state.isSolved()) {
					this -> FINISH_SEARCHING(node.state);
					return;
				}
				if (this -> ITERATION_CHECK() == true) return;
				
				double minClosed = 1e100; bool isMinUpdated = false;
				for (auto [action, newState, cost]: (this -> statement).getAdjacent(node.state)) {
					StateInfo<State> newNode = this -> buildStateInfo(newState, node.g + cost);

					auto itG = (this -> g).find(newState);
					if (itG != (this -> g).end()) {
						double oldG = itG -> second;
						if (oldG <= newNode.g) continue;
						
						StateInfo<State> oldNode = this -> buildStateInfo(newState, oldG);
						openList.erase(oldNode);
						focalList.erase(oldNode);
						closedList.erase(oldNode);
					}

					f[newState] = newNode.f;
					(this -> g)[newState] = newNode.g;
					(this -> actionTrace)[newState] = action;

					if (newNode.f <= node.f + cBound) {
						openList.insert(newNode);
						if (newNode.f <= fMin * eps) 
							focalList.insert(newNode);
					} else {
						minClosed = std::min(minClosed, newNode.f),
						isMinUpdated = true;
					}
				}

				if (not isMinUpdated) 
					closedList.insert(node);
				else if (minClosed < node.f) {
					node.f = f[node.state] = minClosed;
					openList.insert(node);
					if (node.f  <= fMin * eps)
						focalList.insert(node);
				}

				if (not openList.empty()) {
					int fHead = openList.begin() -> f;
					if (fHead > fMin)
						updateFocal(fMin * eps, fHead * eps);
				}
			}
		}
	public:
		double eps, probFocal, cBound;
		ProbabilisticFocalSearch_PartialExpansion(Problem<State> statement, double eps, double probFocal, double cBound): 
			SearchingAlgorithm<State>(statement), eps(eps), probFocal(probFocal), cBound(cBound) 
		{
			this -> algoName = fmt::format("PFS_PE with eps = {}; pFocal = {}, cBound = {}", eps, probFocal, cBound);
		}
};