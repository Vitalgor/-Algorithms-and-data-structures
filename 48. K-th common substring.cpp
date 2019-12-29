#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

class SuffixAutomation{
    public:
    SuffixAutomation():
        nodes_{Node{{}, -1, 0}},
        last_{0}
    {
    }

    bool can_go(long v, char c) const{
        return can_go_(nodes_[v], c);
    }

    long long go(long long v, char c) {
        return nodes_[v].children[c];
    }

    void add(char c) {
        long long new_node = nodes_.size();
        nodes_.push_back(Node{{}, -1, nodes_[last_].maxlen + 1});
        long long curr_node = last_;
        while (!can_go_(nodes_[curr_node], c)){
            nodes_[curr_node].children[c] = new_node;
            curr_node = nodes_[curr_node].suff;
            if (curr_node == -1) {
                break;
            }
        }
        if (curr_node == -1){
            nodes_[new_node].suff = 0;
        } else {
            process_existing_edge_(new_node, curr_node, c);
        }
        last_ = new_node;
    }

    size_t size() const{
        return nodes_.size();
    }

    long long get_max_string_len(size_t i) const{
        return nodes_[i].maxlen;
    }

    long long get_suff_link(size_t i) const{
        return nodes_[i].suff;
    }

private:
    struct Node {
        std::map<char, long long> children;
        long long suff, maxlen;
    };

    std::vector<Node> nodes_;
    long long last_;

    bool can_go_(const Node& n, long long c) const{
        return (n.children.find(c) != n.children.end());
    }

    void process_existing_edge_(long long new_node, long long curr_node, char c) {
        long long next_node = nodes_[curr_node].children[c];
        if (nodes_[curr_node].maxlen + 1 == nodes_[next_node].maxlen) {
            nodes_[new_node].suff = next_node;
        } else {
            long long clone = nodes_.size();
            nodes_.push_back(Node{nodes_[next_node].children, nodes_[next_node].suff,
                             nodes_[curr_node].maxlen + 1});
            while (can_go_(nodes_[curr_node], c) && nodes_[curr_node].children[c] == next_node){
                nodes_[curr_node].children[c] = clone;
                curr_node = nodes_[curr_node].suff;
                if (curr_node == -1) {
                    break;
                }
            }
            nodes_[next_node].suff = clone;
            nodes_[new_node].suff = clone;
        }
    }
};
SuffixAutomation a1 = SuffixAutomation();
SuffixAutomation a2 = SuffixAutomation();

void cnt(map<pair<long, long>, long long>& m, long a, long b){
    m[make_pair(a, b)] = 1;
    for (char c = -128; c != 127; c++) {
        if (!a1.can_go(a, c) || !a2.can_go(b, c)) continue;
        long ag = a1.go(a, c);
        long bg = a2.go(b, c);
        if (m.find(make_pair(ag, bg)) == m.end()) {
            cnt(m, ag, bg);
        }
        m[make_pair(a, b)] += m[make_pair(ag, bg)];
    }
}

void dfs(long long k){
    map<pair<long, long>, long long> m;
    string s = "";
    long a = 0;
    long b = 0;
    cnt(m, a, b);
    //cout << "cnt = " << m[make_pair(0, 0)] << endl;
    if (k >= m[make_pair(0, 0)]){
        cout << "-1" << endl;
        exit(0);
    }
    while (k > 0){
        for (char c = -128; c != 127; c++) {
            if (!a1.can_go(a, c) || !a2.can_go(b, c)) continue;
            long ag = a1.go(a, c);
            long bg = a2.go(b, c);
            //cout << "a,b = " << m[make_pair(ag, bg)] << endl;
            //cout << "k = " << k << endl;
            //cout << "c = " << c << endl;
            if (k > m[make_pair(ag, bg)]) {
                k -= m[make_pair(ag, bg)];
                continue;
            }
            else{
                s += c;
                //cout << "c = " << c << endl;
                //cout << "k = " << k << endl;
                a = ag;
                b = bg;
                k--;
                break;
            }
        }
    }
    cout << s << endl;
    exit(0);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    string s;
    string t;
    cin >> s;
    cin >> t;
    for (auto c: s) {
        a1.add(c);
    }
    for (auto c: t) {
        a2.add(c);
    }
    long long k;
    cin >> k;
    dfs(k);
    return 0;
}
