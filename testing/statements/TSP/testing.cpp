#include "testlib.h"
#include <bits/stdc++.h>
#include "../../../statements/TSP/Problem.h"
#include "../algorithms/problems/TSP.h"
#include "../algorithms/AStar.h"

#define CURRENT_TIMESTAMP std::chrono::steady_clock::now().time_since_epoch().count()
#define MAX_N 20

int n;
Point points[MAX_N];
double d[MAX_N][MAX_N];

double f[1 << MAX_N][MAX_N];

void input();
int main(int argc, char *argv[])
{
	registerGen(argc, argv, 1); rnd.setSeed(CURRENT_TIMESTAMP);
	n = opt<int>("n");
	assert(n <= MAX_N);
	input();
	n--; int ALL = (1 << n) - 1; 
	for (int mask = 0; mask <= ALL; mask++) 
		for (int i = 0; i < n; i++) 
			f[mask][i] = TSP::NaN;

	for (int i = 0; i < n; i++)
		f[1 << i][i] = d[n][i];
	double ans = 1e100;
	for (int mask = 1; mask <= ALL; mask++)
		for (int u = 0; u < n; u++) if ((mask >> u) & 1) {
			if (mask == ALL){
				ans = std::min(ans, f[mask][u] + d[u][n]);
				continue;
			}
			
			for (int v = 0; v < n; v++) if (not ((mask >> v) & 1)) {
				int newMask = mask | (1 << v);
				f[newMask][v] = std::min(f[newMask][v], f[mask][u] + d[u][v]);
			}
		}
	
	std::cout << std::fixed << std::setprecision(6);
	
	std::cerr << Heuristic::MST_Heuristic(Perm(), TSP::n, TSP::adj, TSP::graph) << '\n';
	Problem<Perm> statement = Problem<Perm>(Perm(), Perm());
	AStar algo(statement);
	algo.measure();
	ensure(abs(ans - algo.getAnswer()) < 1e-6);
	std::cout << "answer = " << ans << std::endl;
	std::cout << "output = " << algo.getAnswer() << std::endl;
	std::cout << "time = " << algo.getIterationCount() << std::endl;
	
	
	return 0;
}

void input() {
	TSP::initSize(n);
	
	for (int i = 0; i < n; i++)
		TSP::points[i] = points[i] 
		= {rnd.next(-1'000'000, 1'000'000), rnd.next(-1'000'000, 1'000'000)};
	
	for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
		TSP::adj[i].push_back(j);
		TSP::adj[j].push_back(i);
		TSP::graph[i][j] = TSP::graph[j][i] = d[i][j] = d[j][i] = Point::dist(points[i], points[j]);
	}
}