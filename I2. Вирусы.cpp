#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

typedef struct vertex{
	long next[2] = {-1, -1};
	long go[2] = {-1, -1};
	long parent;
	long sufflink;
	long up;
	char parent_c;
	bool terminal = false;
} vertex;

int go(vertex G[], int v, char c);

int get_link(vertex G[], int v) {
	if (G[v].sufflink == -1)
		if (v == 0 || G[v].parent == 0)
			G[v].sufflink = 0;
		else
			G[v].sufflink = go(G, get_link(G, G[v].parent), G[v].parent_c);
	return G[v].sufflink;
}

int go(vertex G[], int v, char c) {
	if (G[v].go[c] == -1)
		if (G[v].next[c] != -1)
			G[v].go[c] = G[v].next[c];
		else
			G[v].go[c] = v==0 ? 0 : go(G, get_link(G, v), c);
	return G[v].go[c];
}

int up(vertex G[], int v){
    if (G[v].up != -1) return G[v].up;
    if (G[get_link(G, v)].terminal){
		G[v].up = get_link(G, v);
		return G[v].up;
    }
    G[v].up = up(G, get_link(G, v));
	return G[v].up;
}

int bad(vertex G[], int v){
	return (up(G, v) != 0) || (G[v].terminal);
}

int dfs(vertex G[], int v, int visited[]){
    visited[v] = 1;
    for (int c = 0; c < 2; c++){
		long to = go(G, v, c);
		//cout << "v, c = " << v << " " << c << " " << to << endl;
        if (bad(G, to)) continue;
        if (visited[to] == 2) continue;
        if (visited[to] == 1) return 1;
		if (dfs(G, to, visited)) return 1;
    }
    visited[v] = 2;
    return 0;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n;
    long cnt = 1;
    cin >> n;
	vertex G[30000+1];
	G[0].parent = -1;
	G[0].sufflink = -1;
	G[0].terminal = 0;
	G[0].up = 0;
	for (long i = 0; i < n; i++){
		string s;
		cin >> s;
		long v = 0;
		for (size_t j = 0; j < s.length(); ++j) {
			char c = s[j];
			c -= '0';
			if (G[v].next[c] == -1) {
				G[cnt].sufflink = -1;
				G[cnt].up = -1;
				G[cnt].parent = v;
				G[cnt].parent_c = c;
				G[v].next[c] = cnt;
				cnt++;
			}
			v = G[v].next[c];
		}
		cout << "v = " << v << endl;
		G[v].terminal = true;
	}
	int visited[cnt] = {0};
	if (dfs(G, 0, visited))
		cout << "TAK" << endl;
	else
		cout << "NIE" << endl;
    return 0;
}
