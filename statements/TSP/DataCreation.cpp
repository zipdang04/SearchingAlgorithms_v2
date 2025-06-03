
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

#define MAX 1'000'000
int n, m;
Point points[MAX];
std::vector<int> graph[MAX];

std::vector<std::pair<int, int>> edges;
int perm[MAX];

void build() {
	std::iota(perm, perm + n, 0);
	shuffle(perm, perm + n);

	for (int i = 0; i < n; i++) {
		int x = rnd.next(-1'000'000'000, 1'000'000'000);
		int y = rnd.next(-1'000'000'000, 1'000'000'000);
		points[i] = {x, y};
	}
	for (int i = 1; i < n; i++) {
		int p = rnd.next(0, i-1);
		edges.emplace_back(i, p);
	}
	for (int i = n; i <= m; i++) {
		int u = rnd.next(0, n-1);
		int v; do {v = rnd.next(0, n-1);} while (u == v);
		edges.emplace_back(u, v);
	}
}

int main(int argc, char** argv) {
	registerGen(argc, argv, 1);
	rnd.setSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	n = opt<int>("n"); m = opt<int>("m");	
	
	std::string filename = fmt::format("NSQ-n{}-m{}_{}.inp", n, m, getTimeCode());
	std::cerr << "filename: " << filename << std::endl;
	std::ofstream ofs(filename);
	
	build();
	assert((int)edges.size() == m);

	ofs << n << ' ' << m << "\n\n";
	for (int i = 0; i < n; i++) {
		auto [x, y] = points[i];
		ofs << x << ' ' << y << '\n';
	}
	ofs << '\n';
	for (auto [u, v]: edges) {
		u = perm[u], v = perm[v];
		ofs << u << ' ' << v << '\n';
	}


	ofs.flush(); ofs.close();
}