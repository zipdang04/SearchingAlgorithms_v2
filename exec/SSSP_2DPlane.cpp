#include "testlib.h"
#include <bits/stdc++.h>
#include "../statements/SSSP_2DPlane/Problem.h"
#include "../algorithms/problems/SSSP_2DPlane.h"
#include "../algorithms/Dijkstra.h"
#include "../algorithms/AStar.h"
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
	SSSP_2DPlane::input();
}

std::vector<Algorithm*> algorithms;
int main(int argc, char**argv){
	registerGen(argc, argv, 1);
	const int TIME_LIMIT = opt<int>("tl");
	Algorithm::changeTimeLimit(TIME_LIMIT);
	
	input();
	Problem<Node> statement = SSSP_2DPlane::problem;

	double w = 1.1, p = 0.6, C = 1'000'000'000;
	
	algorithms.push_back(new ProbabilisticFocalSearch_PartialExpansion(statement, w, p, C));
	algorithms.push_back(new ProbabilisticFocalSearch(statement, w, p));
	algorithms.push_back(new PFS_VDMPE(statement, w, p));
	algorithms.push_back(new VDM_PE(statement));
	algorithms.push_back(new AStarPartialExpansion(statement, C));
	algorithms.push_back(new FocalSearch(statement, w));
	algorithms.push_back(new AStar(statement));
	for (Algorithm *algorithm: algorithms) {
		std::cerr << algorithm->getName() << std::endl;
		std::cout << "start " << algorithm -> getName() << '\n';
		double timeUsed = algorithm -> measure();
		std::cout << "count of expanded nodes: " << ((SearchingAlgorithm<Node>*) algorithm) -> getExpandedCount() << '\n';
		std::cout << "count of expanded iterations: " << ((SearchingAlgorithm<Node>*) algorithm) -> getIterationCount() << '\n';
		std::cout << "count of max size: " << ((SearchingAlgorithm<Node>*) algorithm) -> getMaxSize() << '\n';
		std::cout << "answer: " << ((SearchingAlgorithm<Node>*) algorithm) -> getAnswer() << '\n';
		std::cout << "steps: ";
		for (std::string s: ((SearchingAlgorithm<Node>*) algorithm) -> getTrace()) std::cout << s << " -> ";
		std::cout << SSSP_2DPlane::n - 1 << '\n';
		// std::cout << "steps: " << ((PuzzleAlgorithm*) algorithm) -> makeStep() << '\n';
		std::cout << "execution time: " << timeUsed << '\n';
		algorithm -> ~Algorithm();
	}
}
