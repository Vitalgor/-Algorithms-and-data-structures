#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
using namespace std;
typedef struct vertex{
	int next[127-32];
	int go[127-32];
	int parent;
	int sufflink;
	int up;
	char parent_c;
	bool terminal = false;
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

int up(vector<vertex> & G, int v){
    if (G[v].up != -1) return G[v].up;
    if (G[get_link(G, v)].terminal){
		G[v].up = get_link(G, v);
		return G[v].up;
    }
    G[v].up = up(G, get_link(G, v));
	return G[v].up;
}

int main(){
    long n;
    long cnt = 1;
    char t[255];
    char *tmp;
    gets(t);
    n = strtol(t, &tmp, 10);
	vector<vertex> G(1);
	G[0].parent = -1;
	G[0].sufflink = -1;
	G[0].terminal = false;
	G[0].up = 0;
	for (long k = 0; k < 127-32; k++){
		G[0].next[k] = -1;
		G[0].go[k] = -1;
	}
	for (long i = 0; i < n; i++){
		char s[255];
		gets(s);
		long v = 0;
		for (size_t j = 0; j < strlen(s); ++j) {
			char c = s[j];
			c -= 32;
			if (G[v].next[c] == -1) {
				vertex u;
				for (long k = 0; k < 127-32; k++){
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
		G[v].terminal = true;
	}
	char s[255];
	while (gets(s)){
		long v = 0;
        for (long i = 0; i < strlen(s); i++){
			char c = s[i];
			c -= 32;
			v = go(G, v, c);
			if (G[v].terminal || up(G, v) != 0){
				puts(s);
				break;
			}
        }
	}
    return 0;
}
