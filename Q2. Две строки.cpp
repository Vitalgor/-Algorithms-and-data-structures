#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <string>
using namespace std;

vector<long> pref(string s){
    long n = s.length();
	vector<long> p(n);
	p[0] = 0;
	for (long i = 1; i < n; i++) {
		int j = p[i-1];
		while (j > 0 && s[i] != s[j])
			j = p[j-1];
		if (s[i] == s[j]) j++;
		p[i] = j;
    }
    return p;
}

string min_str(string s){
    long n = s.length();
	vector<int> z(n);
	z[0] = n;
	for (int i = 1, l = 0, r = 0; i < n; i++) {
		if (i <= r)
			z[i] = min(r-i+1, z[i-l]);
		else
			z[i] = 0;
		while (i+z[i] < n && s[z[i]] == s[i+z[i]])
			z[i]++;
		if (i+z[i]-1 > r)
			l = i,  r = i+z[i]-1;
	}
    for (long len = 1; len <= n; len++){
        int corr = 1;
        int i = 0;
        for (i = 0; i + len < s.length(); i += len){
            if (z[i] < len){
                corr = 0;
                break;
            }
        }
        if (z[i] == (s.length() - i) && corr){
            //cout << len << endl;
            return s.substr(0, len);
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    string s1, s2;
    cin >> s1;
    cin >> s2;
    string s = s1 + s2;
    vector<long> p = pref(s);
    string conc1 = s2;
    //cout << p[s.length()-1] << endl;
    for (long i = p[s.length()-1]; i < s1.length(); i++){
        conc1 += s1[i];
    }
    s = s2 + s1;
    string conc2 = s1;
    p = pref(s);
    for (long i = p[s.length()-1]; i < s2.length(); i++){
        conc2 += s2[i];
    }
    if (s2.find(s1) != string::npos) {
		cout << min_str(s2);
		return 0;
	}
	if (s1.find(s2) != string::npos) {
		cout << min_str(s1);
		return 0;
	}
    string m1 = min_str(conc1);
    string m2 = min_str(conc2);
    if (m1.length() > m2.length()) swap(m1, m2);
    cout << m1;
    return 0;
}
