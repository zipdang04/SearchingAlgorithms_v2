#pragma once

#include <bits/stdc++.h>
#include "Geometry.h"
#include "Perm.h"
#include "heuristic.h"
#include "../Problem.h"
#include <fmt/core.h>
namespace TSP {
	constexpr int START = 0;
	int n, m;
	std::vector<Point> points;
	std::vector<std::unordered_map<int, double>> graph;

	void input() {
		std::cin >> n >> m;
		Perm::changeSize(n);

		points = std::vector<Point>(n);
		for (int i = 0; i < n; i++) {
			int x, y; std::cin >> x >> y;
			points[i] = Point(x, y);
		}
		graph = std::vector<std::unordered_map<int, double>>(n);
		for (int _ = 0; _ < m; _++) {
			int u, v; std::cin >> u >> v;
			double w = Point::dist(points[u], points[v]);
			
			graph[u][v] = w;
			graph[v][u] = w;
		}
	}
}

template<>
double Problem<Perm>::heuristic(Perm node) {
	return Heuristic::MST_Heuristic(node, TSP::n, TSP::graph);
}
template<>
std::vector<Transition<Perm>> Problem<Perm>::getAdjacent(Perm perm) {
	std::vector<Transition<Perm>> all;
	int u = perm.getHead();
	std::string action = fmt::format("{}", u);
	bool end = perm.size() == (TSP::n - 1);
	// std::cerr << "getAdjacent " << perm.toString() << ' ' << end << '\n';
	for (auto [v, w]: TSP::graph[u]) if (not perm.isUsed(v)) {
		Point pV = TSP::points[v];
		double newW = w;
		if (end) {
			if (TSP::graph[v].find(TSP::START) == TSP::graph[v].end()) 
				continue;
			newW += Point::dist(pV, TSP::points[TSP::START]);
		}
		all.push_back(Transition<Perm>(action, perm.add(v), newW));
	}
	// std::cerr << u.get() << "=>";
	// for (Transition<Node>  a: all) std::cerr << fmt::format("({},{},{})", a.action, a.state.get(), a.cost) << ' ';
	// std::cerr << '\n';
	return all;
}