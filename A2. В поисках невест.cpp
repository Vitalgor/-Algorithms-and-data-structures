#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;

typedef struct edge{
    long long to, cap, cost, flow, inv, num;
} edge;
const long long INF = 10000000000;

void ford_bellman(vector<vector<edge>>& G, long long n, pair<long long, long long> parent[], long long dist[]){
	dist[1] = 0;
	bool was = 0;
	for (int i = 1; i <= n; i++){
		was = 0;
		for (int v = 1; v <= n; v++){
			if (dist[v] == INF) continue;
			for (long long j = 0; j < G[v].size(); j++){
				edge e = G[v][j];
				if ((dist[v] + e.cost < dist[e.to]) && (e.cap - e.flow > 0)){
					dist[e.to] = dist[v] + e.cost;
					parent[e.to] = make_pair(v, j);
					was = 1;
				}
			}
		}
	}
	if (was) exit(1);
}

void dijkstra(vector<vector<edge>>& G, long long n, pair<long long, long long> parent[], long long dist[], long long pot[]){
	priority_queue<pair<long long, long long>> Q;
	Q.push(make_pair(0, 1));
	dist[1] = 0;
	while (!Q.empty()) {
		int v = Q.top().second;
		long long cur_d = -Q.top().first;
		Q.pop();
		if (cur_d > dist[v]) continue;
		for (long long j = 0; j < G[v].size(); j++){
			edge e = G[v][j];
			long w = dist[v] + pot[v] - pot[e.to];
			if (e.flow == 0) w += e.cost;
			else w -= e.cost;
			if ((w < dist[e.to]) && (e.cap - e.flow > 0)){
				dist[e.to] = w;
				parent[e.to] = make_pair(v, j);
				Q.push(make_pair(-dist[e.to], e.to));
			}
		}
	}
}

long long count_res(vector<vector<edge>>& G, long long v){
	if (v == G.size()-1) return 0;
    for (long long i = 0; i < G[v].size(); i++){
		if(G[v][i].flow <= 0) continue;
		return 1 + count_res(G, G[v][i].to);
    }
}

void print_res(vector<vector<edge>>& G, long long v){
    if (v == G.size()-1) return;
    for (long long i = 0; i < G[v].size(); i++){
		if(G[v][i].flow <= 0) continue;
		cout << G[v][i].num << " ";
		G[v][i].flow = 0;
		print_res(G, G[v][i].to);
		return;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed);
    long long n, m, k;
    cin >> n >> m >> k;
    vector<vector<edge>> G(n+1);
    for (long long  i = 0; i < m; i++){
		long long  a, b, cost;
		cin >> a >> b >> cost;
		G[a].push_back(edge{b, 1, cost, 0, G[b].size(), i+1});
		G[b].push_back(edge{a, 1, cost, 0, G[a].size()-1, i+1});
	}
	long long pot[n+1], dist[n+1];
	pair<long long, long long> parent[n+1];
	for (long long i = 1; i <= n; i++){
		dist[i] = INF;
		parent[i] = make_pair(-1, -1);
	}
	for (long long i = 1; i <= n; i++){
		pot[i] = 0;
	}
	double res = 0;
	ford_bellman(G, n, parent, dist);
	for (long long i = 0; i < k; i++){
		if (parent[n].first == -1){
			cout << -1 << endl;
			return 0;
		}
		long long v = n;
		while (parent[v].first != -1){
			//cout << v << " " << parent[v].first << endl;
			G[parent[v].first][parent[v].second].flow += 1;
			long long to = v;
			long long inv = G[parent[v].first][parent[v].second].inv;
			G[to][inv].flow -= 1;
			if (G[parent[v].first][parent[v].second].flow == 0){
				res -= G[parent[v].first][parent[v].second].cost;
			}
			else{
				res += G[parent[v].first][parent[v].second].cost;
			}
			v = parent[v].first;
		}
		for (long long i = 1; i <= n; i++){
			dist[i] = INF;
			parent[i] = make_pair(-1, -1);
		}
		dijkstra(G, n, parent, dist, pot);

		for (long long i = 1; i <= n; i++){
			pot[i] = min(pot[i] + dist[i], INF);
		}
		//cout << "----" << endl;
	}
	res /= k;
	cout << res << endl;
	for (int i = 0; i < k; i++){
		cout << count_res(G, 1) << " ";
		print_res(G, 1);
		cout << endl;
	}
    return 0;
}
