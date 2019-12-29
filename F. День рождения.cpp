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
    int tests_cnt = 0;
    cin >> tests_cnt;
    for (long test = 0; test < tests_cnt; test++){
        int n, m;
        cin >> m >> n;
        long cnt = 2*(n+m);
        long s = cnt + 1;
        long t = cnt + 2;
        vector<vector<edge>> G(cnt+3);
        for (long i = 0; i < m; i++){ // юноши
            G[s].push_back(edge{2*i, 1, 0, G[2*i].size()});
            G[2*i].push_back(edge{s, 0, 0, G[s].size()-1});
        }
        for (long i = 0; i < n; i++){ // девушки
            G[t].push_back(edge{2*i+1, 0, 0, G[2*i+1].size()});
            G[2*i+1].push_back(edge{t, 1, 0, G[t].size() - 1});
        }
        int M[m][n];
        for (long i = 0; i < m; i++){
            for (long j = 0; j < n; j++) M[i][j] = 0;
        }
        for (long i = 0; i < m; i++){
            long k;
            cin >> k;
            while (k != 0){
                k--;
                M[i][k] = 1;
                cin >> k;
            }
        }
        for (long i = 0; i < m; i++){
            for (long j = 0; j < n; j++){
                if (!M[i][j]){
                    G[2*i].push_back(edge{2*j + 1, 1, 0, G[2*j + 1].size()});
                    G[2*j+1].push_back(edge{2*i, 0, 0, G[2*i].size()-1});
                }
            }
        }

        long flow = findMaxFlow(G, s, t);
        int visited[cnt];
        for (long j = 0; j < cnt; j++){
            visited[j] = 0;
        }
        dfs_to_find_max_independent_set(G, s, visited);

        long ans_m = 0;
        long ans_f = 0;
        for (long i = 0; i < m; i++){
            if (visited[2*i]) ans_m++;
        }
        for (long i = 0; i < n; i++){
            if (!visited[2*i+1]) ans_f++;
        }
        cout << ans_m + ans_f << endl;
        cout << ans_m << " " << ans_f << endl;
        for (long i = 0; i < m; i++){
            if (visited[2*i]) cout << i+1 << " ";
        }
        cout << endl;
        for (long i = 0; i < n; i++){
            if (!visited[2*i+1]) cout << i+1 << " ";
        }
        cout << endl;
        cout << endl;
    }
    return 0;
}
