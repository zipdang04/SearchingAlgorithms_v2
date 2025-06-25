#pragma once
#include "Perm.h"

namespace Heuristic {
	inline double GapHeuristic(Perm perm) {
		int result = 0;
		for (int i = 0; i + 1 < perm.SIZE; i++) {
			result += abs(perm[i] - perm[i + 1]) > 1;
		}
		return result;
	}
}