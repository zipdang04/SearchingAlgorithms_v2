#pragma once

#include "../base/algorithm.h"
#include "../statements/NSqPuzzle/Problem.h"
#include "../base/exceptions.h"

template<class State>
class SearchingAlgorithm: public Algorithm {
	private:
		bool FINISHED = false;
		long long iteration = 0;
	protected:
		Problem<State> statement;
		std::unordered_map<State, double> g;
		std::unordered_map<State, std::string> actionTrace;

		inline void FINISH_SEARCHING() { FINISHED = true; }
		inline void NEW_ITERATION() {iteration++;}
		
	public:
		SearchingAlgorithm(Problem<State> statement): statement(statement) {}
		
		virtual std::vector<std::string> getTrace() { throw NotImplementedException("no trace function found"); }
		long long getExpandedCount() { return g.size(); }
		long long getIterationCount() { return iteration; }
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
};

class InvalidTraceException: public Exception {
	const std::string PREFIX = "InvalidTraceException";
	public:
		InvalidTraceException(std::string message): Exception(PREFIX, message) {}
};