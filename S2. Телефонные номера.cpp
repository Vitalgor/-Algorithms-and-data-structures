#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <stack>
#include <iomanip>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    while (true){
        string s;
        cin >> s;
        if (s == "-1") return 0;
        long n;
        cin >> n;
        vector<pair<string, string>> dict(n);
        for (int i = 0; i < n; i++){
            cin >> dict[i].first;
            dict[i].second = "";
            for (long j = 0; j < dict[i].first.length(); j++){
                if (dict[i].first[j] == 'i') dict[i].second += '1';
                if (dict[i].first[j] == 'j') dict[i].second += '1';
                if (dict[i].first[j] == 'a') dict[i].second += '2';
                if (dict[i].first[j] == 'b') dict[i].second += '2';
                if (dict[i].first[j] == 'c') dict[i].second += '2';
                if (dict[i].first[j] == 'd') dict[i].second += '3';
                if (dict[i].first[j] == 'e') dict[i].second += '3';
                if (dict[i].first[j] == 'f') dict[i].second += '3';
                if (dict[i].first[j] == 'g') dict[i].second += '4';
                if (dict[i].first[j] == 'h') dict[i].second += '4';
                if (dict[i].first[j] == 'k') dict[i].second += '5';
                if (dict[i].first[j] == 'l') dict[i].second += '5';
                if (dict[i].first[j] == 'm') dict[i].second += '6';
                if (dict[i].first[j] == 'n') dict[i].second += '6';
                if (dict[i].first[j] == 'p') dict[i].second += '7';
                if (dict[i].first[j] == 'r') dict[i].second += '7';
                if (dict[i].first[j] == 's') dict[i].second += '7';
                if (dict[i].first[j] == 't') dict[i].second += '8';
                if (dict[i].first[j] == 'u') dict[i].second += '8';
                if (dict[i].first[j] == 'v') dict[i].second += '8';
                if (dict[i].first[j] == 'w') dict[i].second += '9';
                if (dict[i].first[j] == 'x') dict[i].second += '9';
                if (dict[i].first[j] == 'y') dict[i].second += '9';
                if (dict[i].first[j] == 'o') dict[i].second += '0';
                if (dict[i].first[j] == 'q') dict[i].second += '0';
                if (dict[i].first[j] == 'z') dict[i].second += '0';
            }
            if (dict[i].first.length() != dict[i].second.length()) return 1;
        }
        int len = s.length();
        long last[len+1];
        int prev[len+1];
        int d[len+1];
        for (int i = 0; i <= len; i++){
            last[i] = -1;
            d[i] = -1;
            prev[i] = -1;
        }
        d[0] = 0;
        for (int i = 1; i <= len; i++){
            long min_pos = -1;
            long min_word = -1;
            for (int j = 0; j < n; j++){
                if (dict[j].first.length() > i) continue;
                long pos1 = i - dict[j].second.length();
                long pos2 = 0;
                while (s[pos1] == dict[j].second[pos2]){
                    pos1++;
                    pos2++;
                    if (pos1 == i) break;
                }
                if (pos1 != i) continue;
                if (d[i - dict[j].second.length()] == -1) continue;
                if (min_pos == -1){
                    min_pos = i - dict[j].second.length();
                    min_word = j;
                }
                if (d[min_pos] > d[i - dict[j].second.length()]){
                    min_pos = i - dict[j].second.length();
                    min_word = j;
                }
            }
            if (min_pos != -1){
                d[i] = d[min_pos] + 1;
                last[i] = min_word;
                prev[i] = min_pos;
            }
        }
        if (d[len] == -1) cout << "No solution." << endl;
        else{
            stack<string> S;
            long pos = len;
            for (long i = 0; i < d[len]; i++){
                S.push(dict[last[pos]].first);
                pos = prev[pos];
            }
            while (!S.empty()){
                cout << S.top() << " ";
                S.pop();
            }
            cout << endl;
        }
    }
    return 0;
}
