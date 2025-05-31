#pragma once

#include <bits/stdc++.h>
#include "Geometry.h"
#include "Node.h"
#include "../Problem.h"
#include <fmt/core.h>
namespace SSSP_2DPlane {
	int n, m;
	std::vector<Point> points;
	std::vector<std::vector<int>> graph;

	Problem<Node> problem(0, 0);

	void input() {
		std::cin >> n >> m;
		Node::changeGoal(n-1);

		points = std::vector<Point>(n);
		for (int i = 0; i < n; i++) {
			int x, y; std::cin >> x >> y;
			points[i] = Point(x, y);
		}
		graph = std::vector<std::vector<int>>(n);
		for (int _ = 0; _ < m; _++) {
			int u, v; std::cin >> u >> v;
			graph[u].push_back(v);
			graph[v].push_back(u);
		}
		problem = Problem<Node>(Node(0), Node(n-1));
		
		// for (int i = 0; i < n; i++) std::cerr << fmt::format("({},{})", points[i].x, points[i].y) << " \n"[i == n-1];
	}

	
}

template<>
double Problem<Node>::heuristic(Node node) {
	Point u = SSSP_2DPlane::points[node.get()];
	Point v = SSSP_2DPlane::points[Node::GOAL];
	return Point::dist(u, v);
}
template<>
std::vector<Transition<Node>> Problem<Node>::getAdjacent(Node u) {
	std::vector<Transition<Node>> all;
	Point pU = SSSP_2DPlane::points[u.get()];
	std::string action = fmt::format("{}", u.get());
	for (int v: SSSP_2DPlane::graph[u.get()]) {
		Point pV = SSSP_2DPlane::points[v];
		all.push_back(Transition<Node>(action, Node(v), Point::dist(pU, pV)));
	}
	// std::cerr << u.get() << "=>";
	// for (Transition<Node>  a: all) std::cerr << fmt::format("({},{},{})", a.action, a.state.get(), a.cost) << ' ';
	// std::cerr << '\n';
	return all;
}