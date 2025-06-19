#include "testlib.h"
#include <bits/stdc++.h>
#include "../statements/TSP/Problem.h"
#include "../algorithms/problems/TSP.h"
#include "../algorithms/AStar.h"
#include "../algorithms/Dijkstra.h"
#include "../algorithms/FocalSearch.h"
#include "../algorithms/ProbabilisticFocalSearch.h"
#include "../algorithms/AStarPE.h"
#include "../algorithms/PFS_PE.h"
#include "../algorithms/VDM_PE.h"
#include "../algorithms/PFS_VDMPE.h"

int n;
std::vector<int> state;

void input() {
	std::ios_base::sync_with_stdio(false);
	TSP::input();
}

std::vector<Algorithm*> algorithms;
int main(int argc, char**argv){
	registerGen(argc, argv, 1);
	input();
	Problem<Perm> statement = Problem<Perm>(Perm(), Perm());

	double w = 1.05, p = 0.85, C = 500;
	
	algorithms.push_back(new ProbabilisticFocalSearch_PartialExpansion(statement, w, p, C));
	algorithms.push_back(new ProbabilisticFocalSearch(statement, w, p));
	algorithms.push_back(new PFS_VDMPE(statement, w, p));
	// algorithms.push_back(new FocalSearch(statement, w));
	// algorithms.push_back(new VDM_PE(statement));
	// algorithms.push_back(new AStarPartialExpansion(statement, C));
	// algorithms.push_back(new AStar(statement));
	for (Algorithm *algorithm: algorithms) {
		std::cerr << algorithm->getName() << std::endl;
		std::cout << "start " << algorithm -> getName() << '\n';
		double timeUsed = algorithm -> measure();
		std::cout << "count of expanded nodes: " << ((SearchingAlgorithm<Perm>*) algorithm) -> getExpandedCount() << '\n';
		std::cout << "count of expanded iterations: " << ((SearchingAlgorithm<Perm>*) algorithm) -> getIterationCount() << '\n';
		std::cout << "count of max size: " << ((SearchingAlgorithm<Perm>*) algorithm) -> getMaxSize() << '\n';
		std::cout << "answer: " << ((SearchingAlgorithm<Perm>*) algorithm) -> getAnswer() << '\n';
		std::cout << "steps: ";
		for (std::string s: ((SearchingAlgorithm<Perm>*) algorithm) -> getTrace()) std::cout << s << " -> ";
		std::cout << TSP::START << '\n';
		// std::cout << "steps: " << ((PuzzleAlgorithm*) algorithm) -> makeStep() << '\n';
		std::cout << "execution time: " << timeUsed << '\n';
		algorithm -> ~Algorithm();
	}
}
