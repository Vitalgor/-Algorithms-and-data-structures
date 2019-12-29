//
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;
const long long inf = 100000000000;

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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long long n, m, a, b;
    cin >> n >> m >> a >> b;
    vector<string> s;
    s.resize(n);
    for (int i = 0; i < n; i++){
        cin >> s[i];
        s[i].resize(m);
    }
    //
    vector<vector<edge>> G(n*m + 2);
    long long cnt = 0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if ((i + j) % 2){
                G[n*m].push_back(edge{i*m + j, 1, 0, G[i*m+j].size()});
                G[i*m+j].push_back(edge{n*m, 0, 0, G[n*m].size()-1});
            }
            else{
                G[n*m+1].push_back(edge{i*m + j, 0, 0, G[i*m+j].size()});
                G[i*m+j].push_back(edge{n*m+1, 1, 0, G[n*m+1].size()-1});
            }
            if ((s[i][j] == '*') && ((i + j) % 2)){
                if ((i > 0) && (s[i-1][j] == '*')){
                    G[i*m + j].push_back(edge{(i-1)*m + j, 1, 0, G[(i-1)*m+j].size()});
                    G[(i-1)*m+j].push_back(edge{i*m + j, 0, 0, G[i*m + j].size()-1});
                }
                if ((i < n-1) && (s[i+1][j] == '*')){
                    G[i*m + j].push_back(edge{(i+1)*m + j, 1, 0, G[(i+1)*m+j].size()});
                    G[(i+1)*m+j].push_back(edge{i*m + j, 0, 0, G[i*m + j].size()-1});
                }
                if ((j > 0) && (s[i][j-1] == '*')){
                    G[i*m + j].push_back(edge{i*m + j-1, 1, 0, G[i*m+j-1].size()});
                    G[i*m+j-1].push_back(edge{i*m + j, 0, 0, G[i*m + j].size()-1});
                }
                if ((j < m - 1) && (s[i][j+1] == '*')){
                    G[i*m + j].push_back(edge{i*m + j+1, 1, 0, G[i*m+j+1].size()});
                    G[i*m+j+1].push_back(edge{i*m + j, 0, 0, G[i*m + j].size()-1});
                }
            }
            cnt += (s[i][j] == '*');
        }
    }
    long long match_size = findMaxFlow(G, m*n, m*n+1);
    //cout << match_size << endl;
    if (a >= 2*b) cout << b*cnt;
    else{
        cout << match_size*a + (cnt - 2*match_size)*b;
    }
    return 0;
}
