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
    string s;
	cin >> s;
	long n = s.length();
	vector<int> p(n);
	vector<int> mx(n);
	p[0] = 0;
	mx[0] = 0;
	int curr_max = 0;
	for (long i = 1; i < n; i++) {
		mx[i] = 0;
		int j = p[i-1];
		while (j > 0 && s[i] != s[j])
			j = p[j-1];
		if (s[i] == s[j]) j++;
		p[i] = j;
		if (p[i] > 0){
            mx[i] = mx[p[i]-1] + 1;
		}
        if (mx[i] > mx[curr_max]) curr_max = i;
	}

	for (int i = 0; i <= curr_max; ++i) {
		cout << s[i];
	}
    return 0;
}
