#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    string s, a, b;
    cin >> b >> a;
    s = a + '#' + b;
	long n = s.length();
	vector<int> p(n);
	p[0] = 0;
	long cnt = 0;
	for (long i = 1; i < n; i++) {
		int j = p[i-1];
		while (j > 0 && s[i] != s[j])
			j = p[j-1];
		if (s[i] == s[j]) j++;
		p[i] = j;
		if (p[i] == a.length()) cnt++;
	}
	cout << cnt << endl;
	for (long i = 1; i < n; i++){
		if (p[i] == a.length()) cout << i - 2*a.length() + 1 << " ";
	}
    return 0;
}
