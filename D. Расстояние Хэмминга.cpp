#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <string>
#include <queue>
using namespace std;

long long maxFlow = 0;
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

void dfs(vector<vector<edge>> & G, long long v, char codes[], int visited[]){
    visited[v] = 1;
    for (auto e: G[v]){
        //cout << "want to go: " << v << " " << e.to << "; cap = " << e.cap << " " << e.flow << endl;
        if (visited[e.to]) continue;
        if (e.flow == e.cap) continue;
        long to = e.to;
        //cout << "to = " << to << endl;
        codes[to] = '0';
        dfs(G, to, codes, visited);
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    string st, p;
    cin >> st >> p;
    long n = 2*(st.length() + p.length()) + 2;
    long s = n-1;
    long t = n-2;
    vector<vector<edge>> G(n);
    for (long i = 0; i < p.length(); i++){
        char cp = p[i];
        for (long j = i; j < st.length() - p.length() + i + 1; j++){
            char cs = st[j];
            if (((cp == '0') && (cs == '1')) || ((cp == '1') && (cs == '0'))){
                G[s].push_back(edge{t, 1, 0, G[t].size()});
                G[t].push_back(edge{s, 0, 0, G[s].size()-1});
            }

            if ((cp == '0') && (cs == '?')){
                G[s].push_back(edge{2*j+1, 1, 0, G[2*j+1].size()});
                G[2*j+1].push_back(edge{s, 0, 0, G[s].size()-1});
            }
            if ((cp == '?') && (cs == '0')){
                G[s].push_back(edge{2*i, 1, 0, G[2*i].size()});
                G[2*i].push_back(edge{s, 0, 0, G[s].size()-1});
            }

            if ((cp == '1') && (cs == '?')){
                G[t].push_back(edge{2*j+1, 0, 0, G[2*j+1].size()});
                G[2*j+1].push_back(edge{t, 1, 0, G[t].size()-1});
            }
            if ((cp == '?') && (cs == '1')){
                G[t].push_back(edge{2*i, 0, 0, G[2*i].size()});
                G[2*i].push_back(edge{t, 1, 0, G[t].size()-1});
            }

            if ((cp == '?') && (cs == '?')){
                G[2*j + 1].push_back(edge{2*i, 1, 0, G[2*i].size()});
                G[2*i].push_back(edge{2*j + 1, 1, 0, G[2*j + 1].size()-1});
            }
        }
    }
    long long ans = findMaxFlow(G, s, t);
    cout << ans << endl;
    char codes[n];
    int visited[n];
    for (long i = 0; i < n; i++) codes[i] = '1';
    for (long i = 0; i < n; i++) visited[i] = 0;
    dfs(G, s, codes, visited);
    for (long i = 0; i < st.length(); i++){
        if (st[i] != '?') cout << st[i];
        else cout << codes[2*i+1];
    }
    cout << endl;
    for (long i = 0; i < p.length(); i++){
        if (p[i] != '?') cout << p[i];
        else cout << codes[2*i];
    }
    return 0;
}
