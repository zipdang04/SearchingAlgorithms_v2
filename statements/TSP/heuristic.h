#pragma once
#include <bits/stdc++.h>
#include "Perm.h"
// #include <fmt/core.h>
namespace Heuristic {
	class DSU{
		private:
			int n, scc;
			std::vector<int> dsu;
			int par(int u) { return (dsu[u] < 0) ? u : (dsu[u] = par(dsu[u])); }
		public:
			DSU(int n): n(n), scc(n) { dsu = std::vector<int>(n, -1); }
			inline int countSCC() const { return scc; }
			bool join(int u, int v) {
				u = par(u), v = par(v);
				if (u == v) return false; scc--;
				if (-dsu[u] < -dsu[v]) std::swap(u, v);
				dsu[u] += dsu[v], dsu[v] = u;
				return true;
			}
	};

	inline double MST_Heuristic(Perm perm, int n, const std::vector<std::vector<int>> &adj, const std::vector<std::vector<double>> &graph) {
		std::vector<int> change(n, -1); int ptr = 0;
		std::vector<int> all {perm.getHead()}; 

		change[perm.getHead()] = ptr++;
		if (change[Perm::START] < 0)
			all.push_back(Perm::START),
			change[Perm::START] = ptr++;
		for (int i = 0; i < n; i++) 
			if (not perm.isUsed(i)) 
				change[i] = ptr++,
				all.push_back(i);
		// for (int i: all)  std::cerr << i << ' '; std::cerr << '|';
		
		std::vector<std::tuple<int, int, double>> edges;
		int sz = all.size();
		for (int i = 0; i < sz; i++)
		for (int j = i + 1; j < sz; j++) {
			int u = all[i], v = all[j];
			if (graph[u][v] < 1e100)
				edges.push_back({change[u], change[v], graph[u][v]});
		}
		// for (int u: all) 
		// 	for (int v: adj[u])
		// 		if (u < v and (v == perm.getHead() or v == Perm::START or not perm.isUsed(v))) 
		// 			edges.push_back({change[u], change[v], graph[u][v]});
		sort(edges.begin(), edges.end(), [](std::tuple<int, int, int> a, std::tuple<int, int, int> b) -> bool {
			return std::get<2>(a) < std::get<2>(b);
		});
		// std::cerr << "sorted:\n";
		// for (auto [u, v, w]: edges) std::cerr << fmt::format("({} {} {})", u, v, w) << '\n';

		n = all.size(); DSU dsu(n);
		double answer = 0;
		for (auto [u, v, w]: edges) {
			if (dsu.join(u, v)) answer += w;
			if (dsu.countSCC() == 1) break;
		}
		if (dsu.countSCC() != 1) answer = 1e100;
		// std::cerr << '|' << ' ' << answer << '\n';
		return answer;
	}
}