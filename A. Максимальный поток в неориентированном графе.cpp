#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <list>
#include <iterator>
using namespace std;
//
typedef struct edge{
    long long to, cap, flow, inv;
} edge;

void InitializePreflow(vector<vector<edge>> & G, long long s, long long  h[], long long  e[], long long  n){
    for (long long  u = 1; u <= n; u++){
        h[u] = 0;
        e[u] = 0;
    }
    for (long long  i = 0; i < G[s].size(); i++){
        long long to = G[s][i].to;
        G[s][i].flow = G[s][i].cap;
        G[to][G[s][i].inv].flow = - G[s][i].cap;
        e[to] += G[s][i].cap;
        e[s] -= G[s][i].cap;
    }
    h[s] = n;
}

void push(vector<vector<edge>> & G, long long  u, long long  i, long long  e[]){ // i - номер ребра
    long long d = min(e[u], G[u][i].cap - G[u][i].flow);
    G[u][i].flow += d;
    long long  inv = G[u][i].inv;
    long long  to = G[u][i].to;
    G[to][inv].flow -= d;
    e[u] -= d;
    e[to] += d;
    //cout << "push " << u << " " << to << " " << G[u][i].flow << endl;
}

void relable(vector<vector<edge>> & G, long long  u, long long  h[]){
    long long  minh = -1;
    for (auto e : G[u]){
        if (e.cap > e.flow){
            if ((minh == -1) || (minh > h[e.to])){
                minh = h[e.to];
            }
        }
    }
    //if (minh == -1) cout << "ERROR!" << endl;
    h[u] = minh + 1; //может вызывать ошибку, если minh = -1;
}

void discharge(vector<vector<edge>> & G, long long  u, long long e[], long long  h[], long long  & curr){
    while (e[u] > 0){
        if (curr == G[u].size()){
            relable(G, u, h);
            curr = 0;
        }
        else{
            long long  v = G[u][curr].to;
            if ((G[u][curr].cap > G[u][curr].flow) && (h[u] == h[v] + 1)) push(G, u, curr, e);
            else
                curr++;
        }
    }
}

void RelabelToFront(vector<vector<edge>> & G, long long  s, long long  t, long long  n){
    long long  h[n+1] = {0};
    long long  e[n+1] = {0};
    InitializePreflow(G, s, h, e, n);
    list<long long> L;
    for (long long i = 1; i <= n; i++){
        if ((i != s) && (i != t)) L.push_back(i);
    }
    long long current[n+1] = {0};
    list<long long>::iterator u = L.begin();
    while (u != L.end()){
        long long v = *u;
        long long  oldHeight = h[v];
        discharge(G, v, e, h, current[v]);
        if (h[v] > oldHeight){
            /*
            for (auto i : L){
                cout << i << " ";
            }
            cout << endl;
            */
            L.erase(u);
            L.push_front(v);
            u = L.begin();
            /*
            cout << "u = " << v << endl;
            u = L.begin();
            for (auto i : L){
                cout << i << " ";
            }
            cout << endl;
            cout << "---" << endl;
            */
        }
        u++;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long long  n;
    cin >> n;
    while (n > 0){
        long long  s, t, m;
        cin >> s >> t >> m;
        vector<vector<edge>> G(n+1);
        for (long long  i = 0; i < m; i++){
            long long  a, b, cap;
            cin >> a >> b >> cap;
            G[a].push_back(edge{b, cap, 0, G[b].size()});
            G[b].push_back(edge{a, cap, 0, G[a].size()-1});
        }
        long long max_flow = 0;
        RelabelToFront(G, s, t, n);
        for (auto e: G[s]){
            max_flow += e.flow;
        }
        cout << max_flow << endl;
        cin >> n;
    }
    return 0;
}

