#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef struct vertex{
	int next['z'-'a' + 1];
	int go['z'-'a' + 1];
	int parent;
	int sufflink;
	int up;
	char parent_c;
	bool terminal = false;
	int cnt = 0;
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n;
    cin >> n;
    vector<vertex> G(1);
	G[0].parent = -1;
	G[0].sufflink = -1;
	G[0].terminal = false;
	long abt = 'z'-'a'+1;
	long cnt = 1;
	for (long k = 0; k < abt; k++){
		G[0].next[k] = -1;
		G[0].go[k] = -1;
	}
    for (long t = 0; t < n; t++){
        string s;
        cin >> s;
        if (s[0] >= '0' && s[0] <= '9'){
            long k = std::stoi(s);
            long v = 0;
            while (!G[v].terminal || k != 0){
                char c = 0;
                for (c = 0; c < abt; c++){
                    if (G[v].next[c] == -1) continue;
                    long nx = G[v].next[c];
                    if (G[nx].cnt < k) k -= G[nx].cnt;
                    else break;
                }
                c += 'a';
                cout << c;// << " " << v << " " << k << endl;
                c -= 'a';
                v = G[v].next[c];
                if (G[v].terminal) k -= 1;
            }
            cout << endl;
        }
        else{
            long v = 0;
            for (size_t j = 0; j < s.length(); ++j) {
                char c = s[j];
                c -= 'a';
                G[v].cnt++;
                if (G[v].next[c] == -1) {
                    vertex u;
                    for (long k = 0; k < abt; k++){
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
            G[v].cnt++;
            G[v].terminal = true;
        }
    }
    return 0;
}
