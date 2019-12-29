#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <stack>
using namespace std;

void mul(long a, long long b[]){
    long long p = 0;
    for (long i = 0; i < 7000; i++){
        long long s = b[i]*a + p;
        b[i] = s % 100000000;
        p = s / 100000000;
    }
}

void add(long long a[], long long b[]){
    long long p = 0;
    for (long i = 0; i < 7000; i++){
        long long s = b[i] + a[i] + p;
        a[i] = s % 100000000;
        p = s / 100000000;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long k;
    cin >> k;
    string s;
    cin >> s;
    long n = s.length();
	vector<int> p(n);
	p[0] = 0;
	for (long i = 1; i < n; i++) {
		int j = p[i-1];
		while (j > 0 && s[i] != s[j])
			j = p[j-1];
		if (s[i] == s[j]) j++;
		p[i] = j;
	}
	stack<long> S;
	for (long i = n; i>0; i = p[i-1]) {
      S.push(i);
    }
    long long pow[7000];
    long i = 0;
    long long ans[7000];
    for (long j = 0; j < 7000; j++){
        pow[j] = 0;
        ans[j] = 0;
    }
    pow[0] = 1;
    while (!S.empty()){
        while (i != S.top()){
            mul(k, pow);
            i++;
        }
        S.pop();
        add(ans, pow);
    }
    i = 7000-1;
    while (ans[i] == 0) i--;
    cout << ans[i];
    i--;
    while (i >= 0){
        stack<int> S2;
        for (long j = 0; j < 8; j++){
            S2.push(ans[i] % 10);
            ans[i] /= 10;
        }
        while (!S2.empty()){
            cout << S2.top();
            S2.pop();
        }
        i--;
    }
    return 0;
}
