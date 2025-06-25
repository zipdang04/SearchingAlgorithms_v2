
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
	int SZ_N = opt<int>("n"); Perm::changeSize(SZ_N);

	int shuffle_steps = opt<int>("ctrl", 0);
	
	
	std::string prefix = fmt::format("PCK-n{}", SZ_N);
	if (shuffle_steps > 0) prefix += fmt::format("-ctrl{}", shuffle_steps);
	std::string filename = prefix + fmt::format("_{}.inp", getTimeCode());
	std::cerr << "filename: " << filename << std::endl;
	std::ofstream ofs(filename);
	
	ofs << SZ_N << std::endl;
	
	std::vector<int> board(Perm::SIZE);
	std::iota(board.begin(), board.end(), 0);
	shuffle(board.begin(), board.end());
	std::cerr << Perm::SIZE << ' ' << board.size() << '\n';
	Perm perm(board);
	for (int i = 0; i < Perm::SIZE; i++)
		ofs << perm[i] << " \n"[i == SZ_N - 1];

	ofs.flush(); ofs.close();
}