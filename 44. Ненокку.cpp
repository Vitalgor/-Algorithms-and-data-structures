#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

class SuffixAutomation{
    public:
        SuffixAutomation():
            nodes_{},
            last_{0}
        {
            Node n;
            for (char i = 0; i <= 'z'-'a'+1; i++){
                n.children[i] = -1;
            }
            n.suff = -1;
            n.maxlen = 0;
            nodes_.push_back(n);
        }

    void add(char c) {
		long long new_node = nodes_.size();
		Node n;
		for (char i = 1; i <= 'z'-'a' + 1; i++) {
            n.children[i] = -1;
        }
        n.suff = -1;
        n.maxlen = nodes_[last_].maxlen + 1;
		nodes_.push_back(n);
		long long curr_node = last_;
		while (nodes_[curr_node].children[c] == -1){
            nodes_[curr_node].children[c] = new_node;
            curr_node = nodes_[curr_node].suff;
            if (curr_node == -1) {
                break;
            }
		}
		if (curr_node == -1){
            nodes_[new_node].suff = 0;
        } else {
            long long next_node = nodes_[curr_node].children[c];
            if (nodes_[curr_node].maxlen + 1 == nodes_[next_node].maxlen) {
                nodes_[new_node].suff = next_node;
            } else {
                long long clone = nodes_.size();
                Node n;
                for (char i = 1; i <= 'z'-'a' + 1; i++) {
                    n.children[i] = nodes_[next_node].children[i];
                }
                n.suff = nodes_[next_node].suff;
                n.maxlen = nodes_[curr_node].maxlen + 1;
                nodes_.push_back(n);
                while (nodes_[curr_node].children[c] == next_node){
                    nodes_[curr_node].children[c] = clone;
                    curr_node = nodes_[curr_node].suff;
                    if (curr_node == NO_VALUE_) {
                        break;
                    }
                }
                nodes_[next_node].suff = clone;
                nodes_[new_node].suff = clone;
            }
        }
		last_ = new_node;
	}

	bool accept_word(char s[]){
        long long v = 0;
        for (long long i = 0; i < strlen(s); i++){
            if (nodes_[v].children[s[i]] == -1) return 0;
            v = nodes_[v].children[s[i]];
        }
        return 1;
	}

    private:
        static const long long NO_VALUE_ = -1;
        struct Node {
            long children['z' - 'a' + 2];
            long long suff, maxlen;
        };

        std::vector<Node> nodes_;
        long long last_;
};

int main(){
    long long c;
    SuffixAutomation a = SuffixAutomation();
    while ((c = getc(stdin)) != EOF){
        if (c == 'A'){
            c = getc(stdin);
            char s[100001];
            gets(s);
            long len = strlen(s);
            for(long long i = 0; i < len; i++){
                s[i] = tolower(s[i]);
                s[i] -= 'a';
                s[i] += 1;
                a.add(s[i]);
            }
        }
        else{
            c = getc(stdin);
            char s[51];
            gets(s);
            long len = strlen(s);
            for(long long i = 0; i < len; i++){
                s[i] = tolower(s[i]);
                s[i] -= 'a';
                s[i] += 1;
            }
            if (a.accept_word(s)) puts("YES");
            else puts("NO");
        }
    }
    return 0;
}
