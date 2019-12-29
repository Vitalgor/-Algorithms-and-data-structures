#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;
const long long inf = 10000000000000;
long long maxFlow = 0;

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

long dfs_to_minimize_flow(vector<vector<edge>> & G, long v, long to, int visited[]){
    visited[v] = 1;
    if (v == to) return 1;
    for (int i = 0; i < G[v].size(); i++){
        edge e = G[v][i];
        long u = e.to;
        //cout << "v, u = " << v << " " << u << endl;
        if ((visited[u]) || (e.flow <= 0)) continue;
        long delta = dfs_to_minimize_flow(G, u, to, visited);
        if (delta != 0){
            //cout << "min flow = " << v << " " << u << endl;
            G[v][i].flow -= delta;
            G[u][e.inv].flow += delta;
            return delta;
        }
    }
    return 0;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n, m;
    cin >> n >> m;
    vector<vector<edge>> G(n+1);
    for (long i = 0; i < m; i++){
        long x, y, c;
        cin >> x >> y >> c;
        G[x].push_back(edge{y, c, 0, G[y].size()});
        G[y].push_back(edge{x, 0, 0, G[x].size()-1});
    }
    long s = 1;
    long t = n;
    cout << findMaxFlow(G, s, t) << endl;
    long cnt;
    cin >> cnt;
    for (long i = 0; i < cnt; i++){
        int type, x, y;
        cin >> type >> x >> y;
        if (type == 1){
            for (long i = 0; i < G[x].size(); i++) if (G[x][i].to == y){ G[x][i].cap++; break;}
            cout << findMaxFlow(G, s, t) << endl;
        }
        else{
            for (long i = 0; i < G[x].size(); i++)
            if (G[x][i].to == y){
                if (G[x][i].flow == G[x][i].cap){
                    maxFlow--;
                    G[x][i].flow--;
                    G[y][G[x][i].inv].flow++;
                    int visited[n+1] = {0};
                    for (long i = 0; i <= n; i++) visited[i] = 0;
                    dfs_to_minimize_flow(G, s, x, visited);
                    for (long i = 0; i <= n; i++) visited[i] = 0;
                    dfs_to_minimize_flow(G, y, t, visited);
                }
                G[x][i].cap--;
                break;
            }
            cout << findMaxFlow(G, s, t) << endl;
        }
    }
    return 0;
}
