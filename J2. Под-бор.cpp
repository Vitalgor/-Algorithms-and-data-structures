#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef struct vertex{
	int next['z'-'a'+1];
	int go['z'-'a'+1];
	int parent;
	int sufflink;
	int up;
	char parent_c;
	int w = 0;
	int query = 0;
	vector<int> suffchildren;
} vertex;

int go(vector<vertex> & G, int v, char c);

int get_link(vector<vertex> & G, int v) {
	if (G[v].sufflink == -1)
		if (v == 0 || G[v].parent == 0)
			G[v].sufflink = 0;
		else
			G[v].sufflink = go(G, get_link(G, G[v].parent), G[v].parent_c);
	return G[v].sufflink;
}

int go(vector<vertex> & G, int v, char c) {
	if (G[v].go[c] == -1)
		if (G[v].next[c] != -1)
			G[v].go[c] = G[v].next[c];
		else
			G[v].go[c] = v==0 ? 0 : go(G, get_link(G, v), c);
	return G[v].go[c];
}

void dfs(vector<vertex> & G, long long d[], int v){
    d[v] = G[v].w;
    for (auto u: G[v].suffchildren){
        dfs(G, d, u);
        d[v] += d[u];
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n;
    cin >> n;
    vector<vertex> G(n);
    for (long i = 0; i < n; i++){
        for (long p = 0; p < 'z'-'a'+1; p++){
            G[i].next[p] = -1;
            G[i].go[p] = -1;
        }
        G[i].parent = -1;
        G[i].w = 1;
        G[i].sufflink = -1;
        G[i].up = -1;
    }
    for (long i = 0; i < n; i++){
        long k;
        cin >> k;
        for (long j = 0; j < k; j++){
            long v;
            char c;
            cin >> v >> c;
            v--;
            c -= 'a';
            G[i].next[c] = v;
            G[v].parent = i;
            G[v].parent_c = c;
        }
    }
    long m;
    cin >> m;
    long cnt = n;
    for (long i = 0; i < m; i++){
        string s;
        cin >> s;
		long v = 0;
		for (size_t j = 0; j < s.length(); ++j) {
			char c = s[j];
			c -= 'a';
			if (G[v].next[c] == -1) {
				vertex u;
				for (long k = 0; k < 'z'-'a'+1; k++){
					u.next[k] = -1;
					u.go[k] = -1;
				}
				u.sufflink = -1;
				u.up = -1;
				u.parent = v;
				u.parent_c = c;
				G[v].next[c] = cnt;
				G.push_back(u);
				cnt++;
			}
			v = G[v].next[c];
		}
		G[v].query++;
    }
    long long d[cnt];
    for (long i = cnt-1; i > 0; i--){
        G[get_link(G, i)].suffchildren.push_back(i);
        d[i] = 0;
    }
    d[0] = 0;
    long long ans = 0;
    dfs(G, d, 0);
    for (long i = cnt-1; i > 0; i--){
        if (G[i].query) ans += G[i].query*d[i];
    }
    cout << ans << endl;
    return 0;
}
