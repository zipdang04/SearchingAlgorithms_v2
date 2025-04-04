
#include "testlib.h"
#include <bits/stdc++.h>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include "Problem.h"
std::string getTimeCode() {
	auto curTime = std::chrono::high_resolution_clock::now() += std::chrono::hours(7);
	// auto tmp = fmt::format("{:%F}", curTime);
	return fmt::format("{:%Y%m%d-%H%M%S}", curTime);
}

main(int argc, char** argv) {
	registerGen(argc, argv, 1);
	rnd.setSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	int SZ_N = opt<int>("n"); Board::changeSize(SZ_N);

	int shuffle_steps = opt<int>("ctrl", 0);
	
	
	std::string prefix = fmt::format("NSQ-n{}", SZ_N);
	if (shuffle_steps > 0) prefix += fmt::format("-ctrl{}", shuffle_steps);
	std::string filename = prefix + fmt::format("_{}.inp", getTimeCode());
	std::cerr << "filename: " << filename << std::endl;
	std::ofstream ofs(filename);
	
	ofs << SZ_N << std::endl;
	
	Board state;
	if (shuffle_steps == 0) {
		do {
			std::vector<int> board(SZ_N * SZ_N);
			std::iota(board.begin(), board.end(), 0);
			shuffle(board.begin(), board.end());
			state = Board(board);
		} while (not state.isSolvable());
	} else {
		
		BoardMove all[4] = {BoardMove::GET_D(), BoardMove::GET_L(), BoardMove::GET_U(), BoardMove::GET_R()};
		int prv = 0;
		for (int i = 1; i <= shuffle_steps; i++) {
			
			int nxt;
			do {nxt = rnd.next(0, 3);}
			while (nxt == (prv + 2) % 4);
			prv = nxt;

			try {
				state = state.move(all[nxt]);
			} catch (std::exception e) {}
		}
	}
	for (int i = 0; i < SZ_N; i++)
	for (int j = 0; j < SZ_N; j++)
		ofs << state.get(i, j) << " \n"[j == SZ_N - 1];

	ofs.flush(); ofs.close();
}