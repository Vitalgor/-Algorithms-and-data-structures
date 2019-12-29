#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <queue>
using namespace std;

typedef struct edge{
    long long to, cap, cost, flow, inv;
} edge;
const long long INF = 100000000000;

void dijkstra(vector<vector<edge>>& G, pair<long long, long long> parent[], long long dist[], long long pot[], long long s){
	dist[s] = 0;
	long cnt = G.size();
	int used[cnt] = {0};
	for (int i = 0; i < cnt; i++){
		long v = -1;
		for (long j = 0; j < cnt; j++)
			if (!used[j] && (v == -1 || dist[j] < dist[v])){
				v = j;
			}
		if (dist[v] == INF)
			break;
		used[v] = true;
		for (long long j = 0; j < G[v].size(); j++){
			edge e = G[v][j];
			long long w = dist[v] + e.cost + pot[v] - pot[e.to];
			if ((w < dist[e.to]) && (e.cap - e.flow > 0)){
				dist[e.to] = w;
				parent[e.to] = make_pair(v, j);
			}
		}
	}
}

void ford_bellman(vector<vector<edge>>& G, long long n, pair<long long, long long> parent[], long long dist[], long long s){
	dist[s] = 0;
	bool was = 0;
	for (int i = 1; i <= n; i++){
		was = 0;
		for (int v = 0; v < n; v++){
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

bool kuhn (vector<vector<edge>>& G, long v, int used[], long mt[]){
	if (used[v])
		return false;
	used[v] = true;
	for (auto e: G[v]){
		if (e.flow != 1) continue;
		int to = e.to;
		if (mt[to] == -1 || kuhn(G, mt[to], used, mt)) {
			mt[to] = v;
			return true;
		}
	}
	return false;
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n, k;
    cin >> n >> k;
    vector<vector<edge>> G(2*n+2);
    long s = 2*n;
    long t = 2*n+1;
    long cnt = 2*n+2;
    for (long i = 0; i < n; i++){
		G[s].push_back(edge{2*i, k, 0, 0, G[2*i].size()});
		G[2*i].push_back(edge{s, 0, 0, 0, G[s].size()-1});

		G[2*i+1].push_back(edge{t, k, 0, 0, G[t].size()});
		G[t].push_back(edge{2*i+1, 0, 0, 0, G[2*i+1].size()-1});

		for (long j = 0; j < n; j++){
			long cost;
			cin >> cost;
			G[2*i].push_back(edge{2*j+1, 1, cost, 0, G[2*j+1].size()});
			G[2*j+1].push_back(edge{2*i, 0, -cost, 0, G[2*i].size()-1});
		}
    }

    long long pot[cnt], dist[cnt];
	pair<long long, long long> parent[cnt];
	for (long long i = 0; i < cnt; i++){
		dist[i] = INF;
		parent[i] = make_pair(-1, -1);
	}
	ford_bellman(G, cnt, parent, dist, s);
	for (long long i = 0; i < cnt; i++) pot[i] = dist[i];
	long long res = 0;
	for (long long p = 0; p < k*n; p++){
		//cout << "p = " << p << endl;
		for (long long i = 0; i < cnt; i++){
			dist[i] = INF;
			parent[i] = make_pair(-1, -1);
		}
		dijkstra(G, parent, dist, pot, s);
		long long v = t;
		while (v != s){
			edge e = G[parent[v].first][parent[v].second];
			G[parent[v].first][parent[v].second].flow += 1;
			res += e.cost;
			long long to = v;
			long long inv = e.inv;
			G[to][inv].flow -= 1;
			v = parent[v].first;
		}
		for (long long i = 0; i < cnt; i++){
			pot[i] = pot[i] + dist[i];
		}
	}
	cout << res << endl;
	/*
	for (long i = 0; i < n; i++){
		cout << "i = " << i << endl;
		for (auto e: G[2*i]){
			cout << e.flow << " ";
		}
		cout << endl;
	}
	*/
	for (long j = 0; j < k; j++){
		long mt[cnt];
		for (long v = 0; v < cnt; v++) mt[v] = -1;
		for (int v = 0; v < n; v++) {
			int used[cnt] = {0};
			kuhn(G, 2*v, used, mt);
		}
		long mt2[cnt];
		for (long v = 0; v < n; v++){
			mt2[mt[2*v+1]] = 2*v + 1;
		}
		for (long i = 0; i < n; i++){
			for (long p = 0; p < G[2*i].size(); p++){
                if (G[2*i][p].to == mt2[2*i]){
					G[2*i][p].flow = 0;
					cout << (mt2[2*i] - 1)/2 + 1 << " ";
					break;
                }
			}
		}
		cout << endl;
	}
    return 0;
}
