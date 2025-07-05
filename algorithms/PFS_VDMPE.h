#pragma once

#include "base/SearchingAlgorithm.h"
#include "testlib.h"
#include <fmt/core.h>

template<class State>
class PFS_VDMPE: public SearchingAlgorithm<State> {
	private:
		double fMin = -1e100;
		struct CompareF {
			bool operator() (BigInfo<State> a, BigInfo<State> b) const {
				while (true) {
					if (a.f != b.f) return a.f < b.f;
					if (a.g != b.g) return a.g < b.g;
					if (a.h != b.h) return a.h < b.h;
					if (a.hFocal != b.hFocal) return a.hFocal < b.hFocal;
					if (a.state != b.state) return a.state < b.state;
					if (a.pop_able() != b.pop_able())
						return a.pop_able() < b.pop_able();
					if (not a.pop_able()) break;
					a.pop(); b.pop();
				}
				return false;
			}
		};
		struct CompareH {
			bool operator() (BigInfo<State> a, BigInfo<State> b) const {
				while (true) {
					if (a.hFocal != b.hFocal) return a.hFocal < b.hFocal;
					if (a.h != b.h) return a.h < b.h;
					if (a.g != b.g) return a.g < b.g;
					if (a.f != b.f) return a.f < b.f;
					if (a.state != b.state) return a.state < b.state;
					if (a.pop_able() != b.pop_able())
						return a.pop_able() < b.pop_able();
					if (not a.pop_able()) break;
					a.pop(); b.pop();
				}
				return false;
			}
		};
		std::set<BigInfo<State>, CompareF> openList;
		std::set<BigInfo<State>, CompareH> focalList;

		void updateFocal(double oldBound, double newBound) {
			auto it = openList.lower_bound(BigInfo<State>(State(), oldBound, 0, 0, 0));
			while (it != openList.end() and it -> f <= newBound) {
				focalList.insert(*it); it++;
			}
		}
		
		StateInfo<State> process(BigInfo<State> bigNode) {
			StateInfo<State> node = bigNode;
			if (bigNode.pop_able()) {
				bigNode.pop();
				openList.insert(bigNode);
				if (bigNode.f <= fMin * eps)
				focalList.insert(bigNode);
			}
			return node;
		}
		template<typename Comparator>
		void erase(std::set<BigInfo<State>, Comparator> theSet, StateInfo<State> bound) {
			auto it = theSet.lower_bound(bound);
			if (it == theSet.end()) return;
			if (not (bound == *it)) return;
			BigInfo<State> next = *it; theSet.erase(it);
			
			if (not next.pop_able()) return;
			process(next);
		}

		void execute() override {
			State _start = (this -> statement).getSource();

			(this -> g)[_start] = 0; 
			(this -> actionTrace)[_start] = "";

			BigInfo<State> _init(std::vector<StateInfo<State>>{this -> buildStateInfo(_start, 0)});
			
			openList.insert(_init);
			focalList.insert(_init);

			while (not openList.empty()) {
				this -> UPDATE_SIZE(openList.size() + focalList.size());
				
				fMin = openList.begin() -> f;
				BigInfo<State> bigNode = focalList.empty() ? *openList.begin() : (
					(rnd.next(1.0) < probFocal) ? *focalList.begin() : *openList.begin()
				);
				focalList.erase(bigNode); openList.erase(bigNode);
				
				StateInfo<State> node = process(bigNode);
				if (node.state.isSolved()) {
					this -> FINISH_SEARCHING(node.state);
					return;
				}
				if (node.g != (this -> g[node.state]))
					continue;
				if (this -> ITERATION_CHECK() == true) return;
				
				std::vector<StateInfo<State>> all;
				for (auto [action, newState, cost]: (this -> statement).getAdjacent(node.state)) {
					StateInfo<State> newInfo = this -> buildStateInfo(newState, node.g + cost);

					auto itG = (this -> g).find(newState);
					if (itG != (this -> g).end()) {
						double oldG = itG -> second;
						if (oldG <= newInfo.g) continue;

						// BigInfo<State> _ref(newState, oldG + h, oldG, h);
						// erase(openList, _ref);
						// erase(focalList, _ref);
					}

					(this -> g)[newState] = newInfo.g;
					(this -> actionTrace)[newState] = action;

					all.push_back(newInfo);
					// openList.insert(newNode);
					// if (newNode.f <= fMin * eps) 
					// 	focalList.insert(newNode);
				}
				if (not all.empty()) {
					bigNode = BigInfo(all);
					openList.insert(bigNode);
					if (bigNode.f <= fMin * eps)
						focalList.insert(bigNode);
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
		PFS_VDMPE(Problem<State> statement, double eps, double probFocal): SearchingAlgorithm<State>(statement), eps(eps), probFocal(probFocal) {
			this -> algoName = fmt::format("PFS_VDMPE with eps = {}; pFocal = {}", eps, probFocal);
		}
};