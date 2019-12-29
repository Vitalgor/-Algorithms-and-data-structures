#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;
const long long inf = 1000000000;

typedef struct edge{
    long long to, cap, flow, inv;
} edge;

bool bfs(vector<vector<edge>> & G, long long s, long long t, long long d[]){
    for (long long i = 0; i < G.size(); i++) d[i] = inf;
    d[s] = 0;
    queue<long long> Q;
    Q.push(s);
    while (!Q.empty()){
        long long u = Q.front();
        Q.pop();
        for (auto e: G[u]){
            long long v = e.to;
            if ((e.flow < e.cap) && (d[v] == inf)){
                d[v] = d[u] + 1;
                Q.push(v);
            }
        }
    }
    return d[t] != inf;
}

int dfs(vector<vector<edge>> & G, long long u, long long minC, long long p[], long long d[], long long t){
    if ((u == t) || (minC == 0)) return minC;
    for (; p[u] < G[u].size(); p[u]++){
        long long v = G[u][p[u]].to;
        edge e = G[u][p[u]];
        if (d[v] == d[u] + 1){
            long long delta = dfs(G, v, min(minC, e.cap - e.flow), p, d, t);
            if (delta != 0){
                G[u][p[u]].flow += delta;
                G[v][e.inv].flow -= delta;
                return delta;
            }
        }
    }
    return 0;
}

long long findMaxFlow(vector<vector<edge>> & G, long long s, long long t){
    long long maxFlow = 0;
    long long d[G.size()];
    while (bfs(G, s, t, d)){
        long long p[G.size()] = {0};
        for (long long i = 0; i < G.size(); i++) p[i] = 0;
        long long flow = dfs(G, s, inf, p, d, t);
        //cout << "flow = " << flow << endl;
        while (flow != 0){
            maxFlow += flow;
            flow = dfs(G, s, inf, p, d, t);
        }
    }
    return maxFlow;
}

void bfs_to_find_ans(vector<vector<edge>> G, long s, int visited[]) {
    queue<short int> q;
    q.push(s);
    visited[s] = 1;
    while (!q.empty()) {
        int curVertex = q.front();
        q.pop();
        for (int i = 0; i < G[curVertex].size(); ++i) {
            if (!visited[G[curVertex][i].to] && G[curVertex][i].cap - G[curVertex][i].flow > 0) {
                q.push(G[curVertex][i].to);
                visited[G[curVertex][i].to] = 1;
            }
        }
    }
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n, m;
    cin >> m >> n;
    long A[m][n];
    long l, k;
    cin >> k >> l;
	for (long i = 0; i < m; i++){
		for (long j = 0; j < n; j++){
			A[i][j]	= 0;
		}
	}
    long x, y;
    for (long i = 0; i < k; ++i) {
        cin >> x >> y;
        A[x-1][y-1] = 1;
    }
    for (long i = 0; i < l; ++i) {
        cin >> x >> y;
        A[x-1][y-1] = 2;
    }

    cin >> x >> y;
    long s = (x - 1) * n + y - 1;
    cin >> x >> y;
    long t = (x - 1) * n + y - 1;
    long cnt = 2*m*n;
	long maxnum = m*n;

    vector<vector<edge>> G(cnt);
    int a[4] = {1, -1, 0, 0};
    int b[4] = {0, 0, 1, -1};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (A[i][j] == 1) {
                continue;
            }
            if (A[i][j] == 2) {
                G[n * i + j].push_back(edge{maxnum + n * i + j, 1, 0, G[maxnum + n * i + j].size()});
                G[maxnum + n * i + j].push_back(edge{n * i + j, 0, 0, G[n * i + j].size()-1});
                for (int l = 0; l < 4; ++l) {
                        x = i + a[l];
                        y = j + b[l];
                        if (0 <= x && 0 <= y && x < m && y < n && A[x][y] != 1) {
    		            	G[i * n + j + maxnum].push_back(edge{x * n + y, inf, 0, G[x * n + y].size()});
							G[x * n + y].push_back(edge{i * n + j + maxnum, 0, 0, G[i * n + j + maxnum].size()-1});
                        }
                }
            } else if (A[i][j] == 0) {
                for (int l = 0; l < 4; ++l) {
                        x = i + a[l];
                        y = j + b[l];
                        if (0 <= x && 0 <= y && x < m && y < n && A[x][y] != 1) {
    		            	G[i * n + j].push_back(edge{x * n + y, inf, 0, G[x * n + y].size()});
							G[x * n + y].push_back(edge{i * n + j, 0, 0, G[i * n + j].size()-1});
                        }
                }
            }
        }
    }
    long long maxFlow = findMaxFlow(G, s, t);
    if (maxFlow >= inf) {
        cout << -1;
        return 0;
    }
    cout << maxFlow << endl;

	int visited[cnt];
    for (int i = 0; i < cnt; ++i) {
        visited[i] = 0;
    }
    bfs_to_find_ans(G, s, visited);
    for (int i = 0; i < cnt; ++i) {
        for (int j = 0; j < G[i].size(); j++) {
            if (visited[i] && !visited[G[i][j].to] && (G[i][j].cap - G[i][j].flow == 0) && (G[i][j].cap != 0)){
                cout << i / n + 1 << " " << i % n + 1 << endl;
            }
        }
    }

    return 0;
}
