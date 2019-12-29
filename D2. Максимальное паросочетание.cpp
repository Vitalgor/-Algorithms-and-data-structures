#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

bool kuhn (vector<vector<pair<long, long>>>& G, long v, bool used[], long mt[]){
	if (used[v])
		return false;
	used[v] = true;
	for (auto e: G[v]){
		int to = e.second;
		if (mt[to] == -1 || kuhn(G, mt[to], used, mt)) {
			mt[to] = v;
			return true;
		}
	}
	return false;
}

bool kuhn2 (vector<vector<pair<long, long>>>& G, long v, bool used[], long mt[], long globals[]){
	if (globals[v] == 0) return false;
	if (used[v])
		return false;
	used[v] = true;
	for (auto e: G[v]){
		int to = e.second;
		if (globals[to] == 0) continue;
		if (mt[to] == -1 || kuhn2(G, mt[to], used, mt, globals)) {
			mt[to] = v;
			return true;
		}
	}
	return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n, m, k;
    cin >> n >> m >> k;
    vector<vector<pair<long, long>>> G(2*(n + m + 1));
    vector<pair<long, long>> cost1, cost2;
    long val1[n], val2[m];
    for (long i = 0; i < n; i++){
        long c;
        cin >> c;
        val1[i] = c;
        cost1.push_back(make_pair(-c, i));
    }
    for (long i = 0; i < m; i++){
        long c;
        cin >> c;
        val2[i] = c;
        cost2.push_back(make_pair(-c, i));
    }
    long cnt = 2*(n + m + 1);
    sort(cost1.begin(), cost1.end());
    sort(cost2.begin(), cost2.end());
    bool visited1[cnt], visited2[cnt];
    long mt1[cnt], mt2[cnt];
    long globals[cnt];
    for (long i = 0; i < cnt; i++){
        mt1[i] = -1;
        mt2[i] = -1;
        globals[i] = 0;
    }
    for (long i = 0; i < k; i++){
        long a, b;
        cin >> a >> b;
        a--;
        b--;
        G[2*a].push_back(make_pair(i+1, 2*b+1));
        G[2*b+1].push_back(make_pair(i+1, 2*a));
    }
    for (long i = 0; i < n; i++){
        int v = cost1[i].second;
        for (long i = 0; i < cnt; i++){
            visited1[i] = false;
        }
        kuhn(G, 2*v, visited1, mt1);
    }
    for (long i = 0; i < m; i++){
        int v = cost2[i].second;
        for (long i = 0; i < cnt; i++){
            visited2[i] = false;
        }
        kuhn(G, 2*v+1, visited2, mt2);
    }
    for (int i = 0; i < cnt; i++){
        if (i % 2){
            if (mt1[i] != -1) globals[mt1[i]] = 1;
        }
        else{
            if (mt2[i] != -1) globals[mt2[i]] = 1;
        }
    }
    long mt[cnt];
    for (long i = 0; i < cnt; i++){
        mt[i] = -1;
    }
    /*
    for (long i = 0; i < m; i++){
        if (mt1[2*i+1] != -1)
            cout << i << " " << (mt1[2*i+1])/2 << endl;
    }
    */
    for (long v = 0; v < n; v++){
        for (long i = 0; i < cnt; i++){
            visited2[i] = 0;
        }
        kuhn2(G, 2*v, visited2, mt, globals);
    }
    long cnt1 = 0;
    long long sum = 0;
    for (long i = 0; i < m; i++){
        long to = mt[2*i+1];
        for (auto e: G[2*i+1]){
            if (e.second == to){
                cnt1++;
                sum += val2[i] + val1[to/2];
                break;
            }
        }
    }
    cout << sum << endl;
    cout << cnt1 << endl;
    for (long i = 0; i < m; i++){
        long to = mt[2*i+1];
        for (auto e: G[2*i+1]){
            if (e.second == to){
                cout << e.first << " ";
                break;
            }
        }
    }
    return 0;
}
