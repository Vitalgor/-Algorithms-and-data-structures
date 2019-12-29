#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

typedef struct vec{
    long long x, y;
} vec;

double dist(vec a, vec b){
    return sqrt(0.0 + (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

int compx(vec a, vec b){
    return a.x < b.x;
}

int compy(vec a, vec b){
    return a.y < b.y;
}

double min_dist = 10e10;
long n;

void rec(vec P[], long l, long r){
    if (r - l <= 3){
        for (long i = l; i <= r; ++i){
            for (long j= i+1; j <= r; ++j)
				min_dist = min(min_dist, dist(P[i], P[j]));
        }
		sort(P+l, P+r+1, compy);
		return;
    }

    int m = (l + r) >> 1;
	int midx = P[m].x;
	rec(P, l, m);
	rec (P, m+1, r);
	vec t[n];
	merge(P+l, P+m+1, P+m+1, P+r+1, t, compy);
	copy(t, t+r-l+1, P+l);
	vec P2[n];
	long cnt = 0;
	for (long i = l; i < r; i++){
        if (abs(P[i].x - midx) < min_dist){
            P2[cnt] = P[i];
            cnt++;
        }
	}
	for (long i = 0; i < cnt; i++){
        long j = i + 1;
        while ((j < cnt) && (abs(P2[j].y - P2[i].y) < min_dist)){
            min_dist = min(min_dist, dist(P2[i], P2[j]));
            j++;
        }
	}
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    std::cout.precision(10);
    std::cout.setf(std::ios::fixed);
    cin >> n;
    vec P[n];
    for (long i = 0; i < n; i++){
        long long a, b;
        cin >> a >> b;
        P[i] = vec{a, b};
    }
    sort(P, P+n, compx);
    rec(P,0, n-1);
    cout << min_dist << endl;
    return 0;
}
