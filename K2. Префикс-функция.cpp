#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <string>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
	string s;
	cin >> s;
	long n = s.length();
	vector<int> p(n);
	p[0] = 0;
	cout << 0 << " ";
	for (long i = 1; i < n; i++) {
		int j = p[i-1];
		while (j > 0 && s[i] != s[j])
			j = p[j-1];
		if (s[i] == s[j]) j++;
		p[i] = j;
		cout << p[i] << " ";
	}
    return 0;
}
