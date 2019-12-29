#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

std::vector<int> build_lcp(vector<int>& array_, string s_){
    vector<int> lcp_;
    lcp_.resize(s_.size());
    vector<int> reversed_suff;
    reversed_suff.resize(s_.size());
    for (int i = 0; i < s_.size(); i++) {
        reversed_suff[array_[i]] = i;
    }
    int last_calculated = 0;
    for (int i = 0; i < s_.size(); i++) {
        if (last_calculated) {
            last_calculated--;
        }
        if (reversed_suff[i] == s_.size() - 1) {
            lcp_[s_.size() - 1] = -1;
            last_calculated = 0;
            continue;
        }
        int next_suffix = array_[reversed_suff[i] + 1];
        while (std::max(i + last_calculated, next_suffix + last_calculated) < s_.size() &&
               s_[i + last_calculated] == s_[next_suffix + last_calculated]) {
                    last_calculated++;
               }
        lcp_[reversed_suff[i]] = last_calculated;
    }
    return lcp_;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long n;
    cin >> n;
    string s;
    cin >> s;
    vector<int> suff;
    for (int i = 0; i < n; i++){
        int a;
        cin >> a;
        suff.push_back(a-1);
    }
    vector<int> lcp = build_lcp(suff, s);
    for (int i = 0; i < n-1; i++) cout << lcp[i] << " ";
    return 0;
}
