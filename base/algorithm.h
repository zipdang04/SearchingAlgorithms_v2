#pragma once

#include "exceptions.h"

using duration = std::chrono::_V2::high_resolution_clock::duration;
class Algorithm{
	private:
		inline static const int MICROSECONDS = 1'000'000;
		inline static long long TIME_LIMIT = 600ll * MICROSECONDS;
		duration beginTime, endTime;
		inline duration currentTime() {return std::chrono::high_resolution_clock::now().time_since_epoch();}
		inline long long getDuration() {return std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count();}
	protected:
		// static
		std::string algoName;
		bool hasRun = false;
		
		virtual void execute() {
			throw NotImplementedException("No Algorithm::execute() yet");
		};
		bool isTLE() {
			endTime = currentTime();
			return getDuration() > TIME_LIMIT;
		}
	public:
		std::string getName() {return algoName;}
		Algorithm* changeName(std::string name) {algoName = name; return this;}
		inline static void changeTimeLimit(long long seconds) {TIME_LIMIT = seconds * MICROSECONDS;}

		double measure() {
			beginTime = currentTime();
			execute(); hasRun = true;
			long long count = getDuration();
			return count / 1'000'000.0;
		};
		virtual ~Algorithm() {};
};