#pragma once

#include "StateInfo.h"
#include "../../base/algorithm.h"
#include "../../statements/Problem.h"
#include "../../base/exceptions.h"

template<class State>
class SearchingAlgorithm: public Algorithm {
	private:
		inline static const int TLCHECK_CYCLE = 10'000;
		bool FINISHED = false;
		long long iteration = 0, modulo = 0;
		long long maxSize = 0;
		double answer = 0; State ansState;
	protected:
		Problem<State> statement;
		std::unordered_map<State, double> g;
		std::unordered_map<State, std::string> actionTrace;

		inline void FINISH_SEARCHING(State ansState) { 
			this -> ansState = ansState;
			this -> answer = (this -> g)[ansState];
			FINISHED = true; 
		}
		inline bool ITERATION_CHECK() {
			iteration++; modulo++;
			if (modulo == TLCHECK_CYCLE) return modulo=0, isTLE();	
			else return false;
		}
		inline void UPDATE_SIZE(long long size) {
			if (size > this -> maxSize)
				this -> maxSize = size;
		}

		virtual inline StateInfo<State> buildStateInfo(State state, double g) {
			double h = statement.heuristic(state);
			double f = g + h;
			double hFocal = h;//g + 1.05 * h;
			return StateInfo<State>(state, f, g, h, hFocal);
		}
		
	public:
		SearchingAlgorithm(Problem<State> statement): statement(statement) {}
		virtual std::vector<std::string> getTrace() { throw NotImplementedException("no trace function found"); }
		long long getExpandedCount() { return g.size(); }
		long long getIterationCount() { return iteration; }
		long long getMaxSize() { return maxSize; }
		double getAnsState() { return ansState; }
		double getAnswer() { return answer; }
};



class InvalidTraceException: public Exception {
	inline static const std::string PREFIX = "InvalidTraceException";
	public:
		InvalidTraceException(std::string message): Exception(PREFIX, message) {}
};