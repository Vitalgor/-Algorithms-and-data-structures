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

void dfs_to_fill(vector<vector<edge>> & G, long long v, int visited[]){
    visited[v] = 1;
    visited[v+1] = 1;
    for (auto e: G[v]){
        if (e.cap == 0) continue;
        //cout << "edge " << v << " " << e.to << " " << e.cap << endl;
        long to = e.to;
        if (visited[to]) continue;
        dfs_to_fill(G, (to-1), visited);
    }
}

void dfs_to_find_max_independent_set(vector<vector<edge>> & G, long long v, int visited[]){
    visited[v] = 1;
    for (auto e: G[v]){
        long to = e.to;
        if (visited[to]) continue;
        if (((e.flow == 0) && (e.cap == 1)) || ((e.flow == -1) && (e.cap == 0))){ // ориентируем ребра для поиска вершинного покрытия и независимого множества
            dfs_to_find_max_independent_set(G, to, visited);
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int m;
    cin >> m;
    vector<vector<edge>> G(2*m+2);
    int s = 2*m;
    int t = 2*m + 1;
    int cnt = 2*m + 2;
    int time[m];
    int x1[m];
    int y1[m];
    int x2[m];
    int y2[m];
    for (long i = 0; i < m; i++){
        char h1, h2, m1, m2, _;
        cin >> h1 >> h2 >> _ >> m1 >> m2;
        h1 -= '0';
        h2 -= '0';
        m1 -= '0';
        m2 -= '0';
        time[i] = m2 + 10*m1 + 60*h2 + 600*h1;
        cin >> x1[i] >> y1[i] >> x2[i] >> y2[i];
        G[s].push_back(edge{2*i, 1, 0, G[2*i].size()});
        G[2*i].push_back(edge{s, 0, 0, G[s].size()-1});
        G[2*i + 1].push_back(edge{t, 1, 0, G[t].size()});
        G[t].push_back(edge{2*i+1, 0, 0, G[2*i+1].size()-1});
        for (long j = 0; j < i; j++){
            if (time[j] + abs(x1[j] - x2[j]) + abs(y1[j] - y2[j]) + abs(x1[i] - x2[j]) + abs(y1[i] - y2[j]) < time[i]){
                G[2*j].push_back(edge{2*i+1, 1, 0, G[2*i+1].size()});
                G[2*i+1].push_back(edge{2*j, 0, 0, G[2*j].size()-1});
            }
            if (time[i] + abs(x1[i] - x2[i]) + abs(y1[i] - y2[i]) + abs(x2[i] - x1[j]) + abs(y2[i] - y1[j]) < time[j]){
                G[2*i].push_back(edge{2*j+1, 1, 0, G[2*j+1].size()});
                G[2*j+1].push_back(edge{2*i, 0, 0, G[2*i].size()-1});
            }
        }
    }
    long maxFlow = findMaxFlow(G, s, t);
    int visited[cnt];
    for (long j = 0; j < cnt; j++){
        visited[j] = 0;
    }
    dfs_to_find_max_independent_set(G, s, visited);
    long ans = 0;
    for (long i = 0; i < m; i++){
        if ((visited[2*i]) && (!visited[2*i+1])) ans++;
    }
    cout << ans << endl;
    return 0;
}
