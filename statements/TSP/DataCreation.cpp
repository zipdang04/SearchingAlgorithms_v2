
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

#define MODE_RANDOM "RANDOM"
#define MODE_SINGLE "SINGLE"

void build() {
	std::iota(perm, perm + n, 0);
	shuffle(perm, perm + n);

	for (int i = 0; i < n; i++) {
		int x = rnd.next(-500, 500);
		int y = rnd.next(-500, 500);
		points[i] = {x, y};
	}

	if (opt<bool>("guaranteed")) {
		std::vector<int> all(n);
		std::iota(all.begin(), all.end(), 0);
		shuffle(all.begin(), all.end());
		for (int i = 0; i < n; i++) {
			int j = (i + 1) % n;
			edges.emplace_back(all[i], all[j]);
		}
	}
	else{
		for (int i = 1; i < n; i++) {
			int p = rnd.next(0, i-1);
			edges.emplace_back(i, p);
		}
	}

	if (opt<std::string>("edge_mode") == MODE_RANDOM)
		while ((int)edges.size() < m) {
			int u = rnd.next(0, n-1);
			int v; do {v = rnd.next(0, n-1);} while (u == v);
			edges.emplace_back(u, v);
		}
	else {
		std::set<std::pair<int, int>> all;
		for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++)
			all.insert({i, j});
		for (auto [u, v]: edges) {
			if (u > v) std::swap(u, v);
			all.erase({u, v});
		}
		
		std::vector<std::pair<int, int>> remain(all.begin(), all.end());
		shuffle(remain.begin(), remain.end());

		for (int i = 0; i < remain.size() and edges.size() < m; i++)
			edges.push_back(remain[i]);
		m = edges.size();
	}
}

int main(int argc, char** argv) {
	registerGen(argc, argv, 1);
	rnd.setSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	n = opt<int>("n"); m = opt<int>("m");	
	
	build();

	std::string filename = fmt::format("TSP-n{}-m{}_{}.inp", n, m, getTimeCode());
	std::cerr << "filename: " << filename << std::endl;
	std::ofstream ofs(filename);
	
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