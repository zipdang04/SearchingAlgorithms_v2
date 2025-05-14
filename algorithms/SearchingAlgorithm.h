#pragma once

#include "../base/algorithm.h"
#include "../statements/NSqPuzzle/Problem.h"
#include "../base/exceptions.h"

template<class State>
class SearchingAlgorithm: public Algorithm {
	private:
		bool FINISHED = false;
		long long iteration = 0;
		long long maxSize = 0;
	protected:
		Problem<State> statement;
		std::unordered_map<State, double> g;
		std::unordered_map<State, std::string> actionTrace;

		inline void FINISH_SEARCHING() { FINISHED = true; }
		inline void NEW_ITERATION() {iteration++;}
		inline void UPDATE_SIZE(long long size) {
			if (size > this -> maxSize)
				this -> maxSize = size;
		}
		
	public:
		SearchingAlgorithm(Problem<State> statement): statement(statement) {}
		
		virtual std::vector<std::string> getTrace() { throw NotImplementedException("no trace function found"); }
		long long getExpandedCount() { return g.size(); }
		long long getIterationCount() { return iteration; }
		long long getMaxSize() { return maxSize; }
};

template<class State>
struct StateInfo {
	State state; double f, g, h;
	StateInfo(State state, double f, double g, double h): state(state), f(f), g(g), h(h) {}

	bool operator < (StateInfo a) const {
		return (f != a.f) ? (f < a.f) : (
			(g != a.g) ? (g < a.g) : (state < a.state)
		);
	}
	bool operator > (StateInfo a) const {
		return (f != a.f) ? (f > a.f) : (
			(g != a.g) ? (g > a.g) : (state > a.state)
		);
	}
	bool operator == (StateInfo a) const {
		return state == a.state
			&& f == a.f
			&& g == a.g
			&& h == a.h;
	}
};
template <class State>
class BigInfo: public StateInfo<State> {
	std::queue<StateInfo<State>> unexpanded;
	public:
		BigInfo(State state, double f, double g, double h): StateInfo<State>(state, f, g, h) {}
		BigInfo(StateInfo<State> info): StateInfo<State>(info.state, info.f, info.g, info.h) {}
		BigInfo(std::vector<StateInfo<State>> states): StateInfo<State>(State(), 0, 0, 0)  {
			ensuref(not states.empty(), "list of states are empty");
			std::sort(states.begin(), states.end());

			auto it = states.begin();
			this -> state = it -> state;
			this -> f = it -> f;
			this -> g = it -> g;
			this -> h = it -> h;

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
		}
};

class InvalidTraceException: public Exception {
	inline static const std::string PREFIX = "InvalidTraceException";
	public:
		InvalidTraceException(std::string message): Exception(PREFIX, message) {}
};