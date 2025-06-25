#include "testlib.h"
#include <bits/stdc++.h>
#include "../../../statements/Pancake/Perm.h"
#include "../../../statements/Pancake/Problem.h"
#include "../../../algorithms/problems/Pancake.h"

#define CURRENT_TIMESTAMP std::chrono::steady_clock::now().time_since_epoch().count()

int SIZE;

void TestInit() {
	Perm board = Perm();
	for (int i = 0; i < Perm::SIZE; i++)
		ensure(board[i] == i);
	ensure(board.isSolved());
}

void TestMove() {
	Perm board = Perm();
	
	
	int k = Perm::SIZE / 2;
	board = board.move(k);
	for (int i = 0; i < Perm::SIZE; i++) std::cerr << board[i] << " \n"[i == Perm::SIZE - 1];
	for (int i = 0; i < k; i++)
		ensure(board[i] == k - i - 1);
	for (int i = k; i < Perm::SIZE; i++)
		ensure(board[i] == i);

	Problem<Perm> haha = Problem<Perm>(Perm(), Perm());
	ensure(haha.getAdjacent(Perm()).size() == Perm::SIZE - 1);
	for (auto [action, state, cost]: haha.getAdjacent(Perm())) {
		ensure(cost == 1);
		int k = atoi(action.c_str());
		std::cerr << k << '|';for (int i = 0; i < Perm::SIZE; i++) std::cerr << state[i] << " \n"[i == Perm::SIZE - 1];
		for (int i = 0; i < k; i++)
			ensure(state[i] == k - i - 1);
	}
}

int main(int argc, char** argv) {
	registerGen(argc, argv, 1); rnd.setSeed(CURRENT_TIMESTAMP);
	SIZE = opt<int>("n");
	Perm _INIT(SIZE);

	TestInit();
	TestMove();
}