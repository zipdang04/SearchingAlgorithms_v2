#pragma once
#include <bits/stdc++.h>
#include "Perm.h"
#include <fmt/core.h>
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

	inline double MST_Heuristic(Perm perm, int n, const std::vector<std::unordered_map<int, double>> &graph) {
		std::unordered_map<int, int> change; int ptr = 0;
		std::vector<int> all {perm.getHead(), Perm::START}; 

		change[perm.getHead()] = ptr++;
		for (int i = 0; i < n; i++) 
			if (not perm.isUsed(i)) 
				change[i] = ptr++,
				all.push_back(i);
		
		std::vector<std::tuple<int, int, double>> edges;
		for (int u: all) 
			for (auto [v, w]: graph[u]) 
				if (u < v and (v == perm.getHead() or v == Perm::START or not perm.isUsed(v))) 
					edges.push_back({change[u], change[v], w});
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
		return answer;
	}
}