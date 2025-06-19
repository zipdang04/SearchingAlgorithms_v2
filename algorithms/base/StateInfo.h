#pragma once
#include <bits/stdc++.h>

template<class State>
struct StateInfo {
	State state; double f, g, h, hFocal;
	StateInfo(State state, double f, double g, double h, double hFocal): state(state), f(f), g(g), h(h), hFocal(hFocal) {}

	bool operator < (StateInfo a) const {
		return (f != a.f) ? (f < a.f) : (
			(g != a.g) ? (g < a.g) : (
				(h != a.h) ? (h < a.h) : (
					(hFocal != a.hFocal) ? (hFocal < a.hFocal) : (state < a.state)
				)
			)
		);
	}
	bool operator > (StateInfo a) const {
		return (f != a.f) ? (f > a.f) : (
			(g != a.g) ? (g > a.g) : (
				(h != a.h) ? (h > a.h) : (
					(hFocal != a.hFocal) ? (hFocal > a.hFocal) : (state > a.state)
				)
			)
		);
	}
	bool operator == (StateInfo a) const {
		return state == a.state
			&& f == a.f
			&& g == a.g
			&& h == a.h
			&& hFocal == a.hFocal;
	}
};
template <class State>
class BigInfo: public StateInfo<State> {
	std::queue<StateInfo<State>> unexpanded;
	public:
		BigInfo(State state, double f, double g, double h, double hFocal): StateInfo<State>(state, f, g, h, hFocal) {}
		BigInfo(StateInfo<State> info): StateInfo<State>(info.state, info.f, info.g, info.h, info.hFocal) {}
		BigInfo(std::vector<StateInfo<State>> states): StateInfo<State>(State(), 0, 0, 0, 0)  {
			ensuref(not states.empty(), "list of states are empty");
			std::sort(states.begin(), states.end());

			auto it = states.begin();
			this -> state = it -> state;
			this -> f = it -> f;
			this -> g = it -> g;
			this -> h = it -> h;
			this -> hFocal = it -> hFocal;

			for (auto it = states.begin() + 1; it < states.end(); it++)
				unexpanded.push(*it);
		}
		bool pop_able(){return not unexpanded.empty();}
		void pop() {
			StateInfo<State> newInfo = unexpanded.front(); unexpanded.pop();
			this -> state = newInfo.state;
			this -> f = newInfo.f;
			this -> g = newInfo.g;
			this -> h = newInfo.h;
			this -> hFocal = newInfo.hFocal;
		}
};