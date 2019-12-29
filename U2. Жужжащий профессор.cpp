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
    long long ans = 0;
    string s;
    cin >> s;
    long len = s.length();
    long longest[len];
    for (long i = 0; i < len; i++){
        longest[i] = 0;
    }
    for (long f = 0; f < len; f++){
        long n = s.length();
        vector<long> z(n);
        z[0] = n;
        for (long i = 1, l = 0, r = 0; i < n; i++) {
            if (i <= r)
                z[i] = min(r-i+1, z[i-l]);
            else
                z[i] = 0;
            while (i+z[i] < n && s[z[i]] == s[i+z[i]])
                z[i]++;
            if (i+z[i]-1 > r)
                l = i,  r = i+z[i]-1;
        }
        long ans1 = 0;
        //cout << "f = " << f << " " << longest[f] << endl;
        for (long i = 1; i < n; i++){
            long zm = min(z[i], i);
            if (longest[f] >= zm || longest[f+i] >= zm){
                longest[i+f] = max(zm, longest[i+f]);
                continue;
            }
            ans += zm - max(longest[f], longest[f+i]);
            ans1 += zm - max(longest[f], longest[f+i]);
            //cout << "i = " << i << " " << zm << " " << longest[i+f] << endl;
            longest[f] = zm;
            longest[i+f] = zm;
        }
        //cout << f << " " << ans1 << " " << longest[f] << endl;
        s = s.substr(1);
    }
    cout << ans << endl;
    return 0;
}
