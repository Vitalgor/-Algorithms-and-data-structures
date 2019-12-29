#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <stack>
using namespace std;

struct line{
    long long k, b;
    long double l, r;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long long n;
    cin >> n;
    vector<pair<long long, long long>> A(n);
    for (long i = 0; i < n; i++) {
        cin >> A[i].first >> A[i].second;
    }
    sort(A.begin(), A.end());
    vector<pair<long long, long long>> B;
    for (auto a: A){
        while (!B.empty()) {
            if ((a.first >= B.back().first) && (a.second >= B.back().second))
                B.pop_back();
            else
                break;
        }
        B.push_back(a);
    }
    n = B.size();
    long long dp[n+1];
    dp[0] = 0;
    vector<line> V;
    long long inf = 1000000000000000;
    V.push_back({B[0].second, 0, -inf, inf});
    for (long long i = 1; i <= n; i++) {
        long long l = 0;
        long long r = V.size() - 1;
        dp[i] = -1;
        while (l < r) {
            long long m = (l + r) / 2;
            if (V[m].l > B[i-1].first) {
                r = m - 1;
                continue;
            }
            if (V[m].r < B[i-1].first) {
                l = m + 1;
                continue;
            }
            dp[i] = V[m].k*B[i-1].first + V[m].b;
            break;
        }
        if (dp[i] == -1) {
            dp[i] = V[l].k*B[i-1].first + V[l].b;
        }
        long long k = B[i].second;
        long long b = dp[i];
        r = inf;
        while (V.size() > 0) {
            auto a = V.back();
            long double x = (b - a.b)*1./ (a.k - k);
            if (x <= a.l) {
                V.pop_back();
            }
            else {
                V[V.size()-1].r = x;
                l = x;
                V.push_back({k, b, l, r});
                break;
            }
        }
    }
    cout << dp[n];
    return 0;
}
