#include "testlib.h"
#include <bits/stdc++.h>
#include "../statements/Pancake/Problem.h"
#include "../algorithms/problems/Pancake.h"
#include "../algorithms/Dijkstra.h"
#include "../algorithms/AStar.h"
#include "../algorithms/FocalSearch.h"
#include "../algorithms/ProbabilisticFocalSearch.h"
#include "../algorithms/AStarPE.h"
#include "../algorithms/PFS_PE.h"
#include "../algorithms/VDM_PE.h"
#include "../algorithms/PFS_VDMPE.h"
// const int SIZE = 4;
Perm _INIT("NOT_INIT");

int n;
std::vector<int> state;

void input() {
	std::ios_base::sync_with_stdio(false);
	std::cin >> n; 
	state.resize(n); Perm::changeSize(n);
	
	for (int& i: state) std::cin >> i;
}

std::vector<Algorithm*> algorithms;
int main(int argc, char**argv){
	registerGen(argc, argv, 1);
	const int TIME_LIMIT = opt<int>("tl");
	Algorithm::changeTimeLimit(TIME_LIMIT);
	input();

	double w = 1.1, p = 0.6, C = 2;

	Problem<Perm> statement = Problem<Perm>(Perm(state), Perm());
	algorithms.push_back(new ProbabilisticFocalSearch_PartialExpansion(statement, w, p, C));
	algorithms.push_back(new ProbabilisticFocalSearch(statement, w, p));
	algorithms.push_back(new PFS_VDMPE(statement, w, p));
	// algorithms.push_back(new VDM_PE(statement));
	// algorithms.push_back(new AStarPartialExpansion(statement, 2));
	// algorithms.push_back(new FocalSearch(statement, 1.05));
	// algorithms.push_back(new AStar(statement)); 
	for (Algorithm *algorithm: algorithms) {
		std::cerr << algorithm->getName() << std::endl;
		std::cout << "start " << algorithm -> getName() << '\n';
		double timeUsed = algorithm -> measure();
		std::cout << "count of expanded nodes: " << ((SearchingAlgorithm<Perm>*) algorithm) -> getExpandedCount() << '\n';
		std::cout << "count of expanded iterations: " << ((SearchingAlgorithm<Perm>*) algorithm) -> getIterationCount() << '\n';
		std::cout << "count of max size: " << ((SearchingAlgorithm<Perm>*) algorithm) -> getMaxSize() << '\n';
		
		std::vector<std::string> trace = ((SearchingAlgorithm<Perm>*) algorithm) -> getTrace();
		std::cout << "answer length: " << trace.size() << '\n';
		std::cout << "steps: ";
		for (std::string s: ((SearchingAlgorithm<Perm>*) algorithm) -> getTrace()) std::cout << s << " -> ";
		std::cout << '\n';
		// std::cout << "steps: " << ((PuzzleAlgorithm*) algorithm) -> makeStep() << '\n';
		std::cout << "execution time: " << timeUsed << '\n';
		algorithm -> ~Algorithm();
	}
}
