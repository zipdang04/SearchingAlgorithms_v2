#pragma once

#include <bits/stdc++.h>
#include "Geometry.h"
#include "Perm.h"
#include "heuristic.h"
#include "../Problem.h"
namespace TSP {
	constexpr int START = 0;
	constexpr double NaN = 1e100;
	int n, m = 0;
	std::vector<Point> points;
	std::vector<std::vector<int>> adj;
	std::vector<std::vector<double>> graph;

	void initSize(int _n) {
		n = _n;
		Perm::changeSize(n);
		points = std::vector<Point>(n);
		adj = std::vector<std::vector<int>>(n);
		graph = std::vector<std::vector<double>>(n, std::vector<double>(n, NaN));
	}
	void input() {
		std::cin >> n >> m;
		initSize(n);
		for (int i = 0; i < n; i++) {
			double x, y; std::cin >> x >> y;
			points[i] = Point(x, y);
		}
		for (int _ = 0; _ < m; _++) {
			int u, v; std::cin >> u >> v;
			double w = Point::dist(points[u], points[v]);
			
			graph[u][v] = w;
			graph[v][u] = w;
		}
		for (int u = 0; u < n; u++) 
			for (int v = 0; v < n; v++)
				if (u != v and graph[u][v] < NaN)
					adj[u].push_back(v);
	}
}

template<>
double Problem<Perm>::heuristic(Perm node) {
	return Heuristic::MST_Heuristic(node, TSP::n, TSP::adj, TSP::graph);
}
template<>
std::vector<Transition<Perm>> Problem<Perm>::getAdjacent(Perm perm) {
	std::vector<Transition<Perm>> all;
	int u = perm.getHead();
	std::string action = std::to_string(u);
	bool end = perm.size() == (TSP::n - 1);
	// std::cerr << "getAdjacent " << perm.toString() << ' ' << end << '\n';
	for (int v: TSP::adj[u]) if (not perm.isUsed(v)) {
		Point pV = TSP::points[v];
		double newW = TSP::graph[u][v];
		if (end) {
			if (TSP::graph[v][TSP::START] == TSP::NaN) 
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