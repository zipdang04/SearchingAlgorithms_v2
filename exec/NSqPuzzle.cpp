#include "testlib.h"
#include <bits/stdc++.h>
#include "../statements/NSqPuzzle/Problem.h"
#include "../algorithms/problems/NSqPuzzle.h"
#include "../algorithms/Dijkstra.h"
#include "../algorithms/AStar.h"
#include "../algorithms/FocalSearch.h"
#include "../algorithms/ProbabilisticFocalSearch.h"
#include "../algorithms/AStarPE.h"
#include "../algorithms/PFS_PE.h"
#include "../algorithms/VDM_PE.h"
#include "../algorithms/PFS_VDMPE.h"
// const int SIZE = 4;
Board _INIT("NOT_INIT");

int n;
std::vector<int> state;

void input() {
	std::ios_base::sync_with_stdio(false);
	std::cin >> n; 
	state.resize(n * n); Board::changeSize(n);
	
	for (int& i: state) std::cin >> i;
}

std::vector<Algorithm*> algorithms;
int main(int argc, char**argv){
	registerGen(argc, argv, 1);
	input();

	Problem<Board> statement = Problem<Board>(Board(state), Board());
	algorithms.push_back(new ProbabilisticFocalSearch_PartialExpansion(statement, 1.05, 0.6, 2));
	// algorithms.push_back(new ProbabilisticFocalSearch(statement, 1.05, 0.6));
	// algorithms.push_back(new PFS_VDMPE(statement, 1.05, 0.6));
	// algorithms.push_back(new VDM_PE(statement));
	// algorithms.push_back(new AStarPartialExpansion(statement, 2));
	// algorithms.push_back(new FocalSearch(statement, 1.05));
	// algorithms.push_back(new AStar(statement));
	for (Algorithm *algorithm: algorithms) {
		std::cerr << algorithm->getName() << std::endl;
		std::cout << "start " << algorithm -> getName() << '\n';
		double timeUsed = algorithm -> measure();
		std::cout << "count of expanded nodes: " << ((SearchingAlgorithm<Board>*) algorithm) -> getExpandedCount() << '\n';
		std::cout << "count of expanded iterations: " << ((SearchingAlgorithm<Board>*) algorithm) -> getIterationCount() << '\n';
		std::cout << "count of max size: " << ((SearchingAlgorithm<Board>*) algorithm) -> getMaxSize() << '\n';
		std::cout << "steps: ";
		for (std::string s: ((SearchingAlgorithm<Board>*) algorithm) -> getTrace()) std::cout << s;
		std::cout << '\n';
		// std::cout << "steps: " << ((PuzzleAlgorithm*) algorithm) -> makeStep() << '\n';
		std::cout << "execution time: " << timeUsed << '\n';
		algorithm -> ~Algorithm();
	}
}
