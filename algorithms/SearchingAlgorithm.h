#pragma once

#include "../base/algorithm.h"
#include "../statements/NSqPuzzle/Problem.h"
#include "../base/exceptions.h"

template<class State>
class SearchingAlgorithm: public Algorithm {
	private:
		bool FINISHED = false;
	protected:
		Problem<State> statement;
		std::unordered_map<State, int> g;
		std::unordered_map<State, std::string> actionTrace;

		void FINISH_SEARCHING() { FINISHED = true; }
		
	public:
		SearchingAlgorithm(Problem<State> statement): statement(statement) {}
		
		virtual std::vector<std::string> getTrace() { throw NotImplementedException("no trace function found"); }
		long long getExpandedCount() { return g.size(); }
};

template<class State>
struct StateInfo {
	State state; int f, g, h;
	StateInfo(State state, int f, int g, int h): state(state), f(f), g(g), h(h) {}

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