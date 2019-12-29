#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>

namespace NStringStructures {
    struct data {
        std::vector<size_t> str;
        size_t len, maxnum;
    };

    struct Answer {
        std::vector<size_t> str;
        size_t refren, len;
    };

    class SuffixAutomation{
    public:
        SuffixAutomation():
            nodes_{Node{{}, NO_VALUE_, 0}},
            last_{0}
        {
        }

        void add(int c) {
            int new_node = nodes_.size();
            nodes_.push_back(Node{{}, NO_VALUE_, nodes_[last_].maxlen + 1});
            int curr_node = last_;
            while (!can_go_(nodes_[curr_node], c)){
                nodes_[curr_node].children[c] = new_node;
                curr_node = nodes_[curr_node].suff;
                if (curr_node == NO_VALUE_) {
                    break;
                }
            }
            if (curr_node == NO_VALUE_){
                nodes_[new_node].suff = 0;
            } else {
                process_existing_edge_(new_node, curr_node, c);
            }
            last_ = new_node;
        }

        size_t size() const{
            return nodes_.size();
        }

        int get_max_string_len(size_t i) const{
            return nodes_[i].maxlen;
        }

        int get_suff_link(size_t i) const{
            return nodes_[i].suff;
        }

    private:
        static const int NO_VALUE_ = -1;
        struct Node {
            std::map<int, int> children;
            int suff, maxlen;
        };

        std::vector<Node> nodes_;
        int last_;

        bool can_go_(Node& n, int c) const{
            return (n.children.find(c) != n.children.end());
        }

        void process_existing_edge_(int new_node, int curr_node, int c) {
            int next_node = nodes_[curr_node].children[c];
            if (nodes_[curr_node].maxlen + 1 == nodes_[next_node].maxlen) {
                nodes_[new_node].suff = next_node;
            } else {
                int clone = nodes_.size();
                nodes_.push_back(Node{nodes_[next_node].children, nodes_[next_node].suff,
                                 nodes_[curr_node].maxlen + 1});
                while (can_go_(nodes_[curr_node], c) && nodes_[curr_node].children[c] == next_node){
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
    };

    class SuffixArray{
    public:
        SuffixArray():
            s_{},
            array_{},
            classes_{},
            lcp_{}
        {
        }

        std::vector<size_t> build_array(const std::vector<size_t>& str){
            s_ = str;
            s_.push_back(0);
            init_();
            long power2 = 1;
            for (int i = 0; power2 < array_.size(); i++) {
                phase_(i);
                power2 *= 2;
            }
            array_.erase(array_.begin());
            s_.pop_back();
            return array_;
        }

        std::vector<size_t> build_lcp(){
            if (array_.size() == 0) {
                build_array(s_);
            }
            lcp_.resize(s_.size());
            std::vector<size_t> reversed_suff;
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

    private:
        std::vector<size_t> s_;
        std::vector<size_t> array_, classes_, lcp_;
        static const size_t alphabet_size = 11;

        void init_(){
            array_.resize(s_.size());
            classes_.resize(s_.size());
            int cnt[alphabet_size];
            for (size_t i = 0; i < alphabet_size; i++) {
                cnt[i] = 0;
            }
            for (size_t i = 0; i < s_.size(); i++) {
                cnt[s_[i]]++;
            }
            for (size_t i = 1; i < alphabet_size; i++) {
                cnt[i] += cnt[i-1];
            }
            for (size_t i = 0; i < s_.size(); i++) {
                cnt[s_[i]]--;
                array_[cnt[s_[i]]] = i;
            }
            classes_[array_[0]] = 0;
            size_t classes_cnt = 1;
            for (size_t i = 1; i < s_.size(); i++) {
                if (s_[array_[i]] != s_[array_[i - 1]]) {
                    classes_cnt++;
                }
                classes_[array_[i]] = classes_cnt - 1;
            }
        }

        std::vector<size_t> order_substrings_by_second_part_(size_t phase_number) {
            std::vector<size_t> substr_ordered_by_second_part(s_.size());
            for (size_t i = 0; i < s_.size(); i++) {
                substr_ordered_by_second_part[i] = array_[i] - (1 << phase_number);
                if (substr_ordered_by_second_part[i] < 0) {
                    substr_ordered_by_second_part[i] += s_.size();
                }
            }
            return substr_ordered_by_second_part;
        }

        std::vector<size_t> order_substrings_by_first_part_(const std::vector<size_t>& substr_ordered_by_second_part) {
            size_t cnt[classes_.size()];
            for (size_t i = 0; i < classes_.size(); i++) {
                cnt[i] = 0;
            }
            for (size_t i = 0; i < s_.size(); i++) {
                cnt[classes_[substr_ordered_by_second_part[i]]]++;
            }
            for (size_t i = 1; i < classes_.size(); i++) {
                cnt[i] += cnt[i-1];
            }
            for (size_t i = s_.size() - 1; i >= 0; i--){
                cnt[classes_[substr_ordered_by_second_part[i]]]--;
                array_[cnt[classes_[substr_ordered_by_second_part[i]]]] = substr_ordered_by_second_part[i];
            }
        }

        void phase_(size_t phase_number) {
            std::vector<size_t> substr_ordered_by_second_part = order_substrings_by_second_part_(phase_number);
            order_substrings_by_first_part_(substr_ordered_by_second_part);
            classes_ = build_new_classes_(phase_number);
        }

        std::vector<size_t> build_new_classes_(size_t phase_number) {
            std::vector<size_t> new_classes;
            new_classes.resize(array_.size());
            new_classes[array_[0]] = 0;
            size_t classes_cnt = 1;
            for (size_t i = 1; i < s_.size(); i++) {
                size_t first_class1 = (array_[i] + (1 << phase_number)) % s_.size();
                size_t first_class2 = (array_[i-1] + (1 << phase_number)) % s_.size();
                if (classes_[array_[i]] != classes_[array_[i - 1]] || classes_[first_class1] != classes_[first_class2]) {
                    classes_cnt++;
                }
                new_classes[array_[i]] = classes_cnt-1;
            }
            return new_classes;
        }
    };

    class SuffixTree {
    public:
        explicit SuffixTree(size_t inf = 150000, size_t alphabet_size = 10)
            :nodes_{ Node{ {}, 1, 0, 1, 0},
                     Node{ {}, NO_VALUE_, NO_VALUE_, NO_VALUE_, 0} },
             pos_last_not_leaf_{0, 0},
             s_{},
             inf_(inf)
        {
            for (size_t c = 0; c <= alphabet_size; c++)
                nodes_[1].children[c] = Position{ 0, 1 };
        }

        size_t size() const{
            return nodes_.size();
        }

        size_t length(size_t node_number) const{
            size_t parent = nodes_[node_number].parent;
            size_t parent_symbol = nodes_[node_number].parent_symbol;
            return nodes_[parent].children.at(parent_symbol).dist;
        }

        size_t substr_end(int node_number) const{
            return nodes_[node_number].substr_end;
        }

        void set_inf(size_t inf) {
            inf_ = inf;
        }

        size_t child(size_t node, size_t c) {
            if (!can_go_(Position{node, 0}, c)) {
                return NO_VALUE_;
            }
            return nodes_[node].children[c].node_number;
        }

        bool is_leaf(size_t node) const{
            return !nodes_[node].children.size();
        }

        void add_symbol(size_t c) {
            while (!can_go_(pos_last_not_leaf_, c)) {
                size_t new_node = build_node_if_need_(pos_last_not_leaf_);
                nodes_[new_node].children[c] = Position{ nodes_.size(), inf_ - s_.size() };
                nodes_.push_back(Node{ {}, NO_VALUE_, inf_, new_node, c });
                pos_last_not_leaf_ = Position{nodes_[new_node].suff, 0};
            }
            pos_last_not_leaf_ = go_(pos_last_not_leaf_, c);
            s_.push_back(c);
        }
    private:
        static const int NO_VALUE_ = -1;
        struct Position {
            size_t node_number, dist;
	    };

	    struct Node {
            std::map<size_t, Position> children;
            int suff, substr_end, parent;
            size_t parent_symbol;
        };

        std::vector<Node> nodes_;
        Position pos_last_not_leaf_;
        std::vector<size_t> s_;
        size_t inf_;

        bool is_vertex_(const Position& pos) const {
            return pos.dist == 0;
        }

        int get_symbol_(const Position& pos) const {
            return s_[nodes_[pos.node_number].substr_end - pos.dist];
        }

        bool can_go_(Position pos, int c) const{
            if (is_vertex_(pos))
                return nodes_[pos.node_number].children.find(c) != nodes_[pos.node_number].children.end();
            else
                return c == get_symbol_(pos);
        }

        Position go_(Position pos, size_t c) {
            Position next_pos = pos;
            if (is_vertex_(next_pos))
                next_pos = nodes_[next_pos.node_number].children[c];
            next_pos.dist--;
            return next_pos;
        }

        int suffix_link_(size_t node_number) {
            int parent = nodes_[node_number].parent;
            Position parent_link{nodes_[parent].suff, 0};
            size_t end_str_pos = nodes_[node_number].substr_end;
            size_t begin_str_pos = end_str_pos - nodes_[parent].children[nodes_[node_number].parent_symbol].dist;
            Position link = parent_link;
            while (begin_str_pos < end_str_pos) {
                if (is_vertex_(link))
                    link = nodes_[link.node_number].children[s_[begin_str_pos]];
                size_t len = std::min(end_str_pos - begin_str_pos, link.dist);
                begin_str_pos += len;
                link.dist -= len;
            }

            return build_node_if_need_(link);
        }

        int build_node_if_need_(Position pos) {
            if (is_vertex_(pos))
                return pos.node_number;
            size_t node = pos.node_number;
            size_t parent = nodes_[node].parent;
            size_t new_node = nodes_.size();
            size_t c = get_symbol_(pos);
            size_t parent_c = nodes_[node].parent_symbol;
            nodes_.push_back(Node{{}, NO_VALUE_, nodes_[node].substr_end - pos.dist, parent, parent_c});
            nodes_[new_node].children[c] = pos;
            nodes_[parent].children[parent_c].dist -= pos.dist;
            nodes_[parent].children[parent_c].node_number = new_node;
            int suff_link = suffix_link_(new_node);
            nodes_[new_node].suff = suff_link;
            nodes_[node].parent = new_node;
            nodes_[node].parent_symbol = c;
            return new_node;
        }
    };

    class TreeSolver {
    public:
        TreeSolver(data input):
            len_{input.len},
            max_prod_{0},
            max_num_{input.maxnum},
            best_values_{std::make_pair(0, 0)},
            sequence_{input.str},
            tree_{}
        {
        }

        Answer solve() {
            tree_.set_inf(len_ + 1);
            for (int i = 0; i < len_; i++){
                tree_.add_symbol(sequence_[i]);
            }
            tree_.add_symbol(0);
            int sz = tree_.size();
            max_prod_ = len_;
            best_values_ = std::make_pair(0, len_);
            dfs_(0, 0);
            Answer ans;
            ans.refren = max_prod_;
            ans.len = best_values_.second;
            for (int i = best_values_.first; i < best_values_.first + best_values_.second; i++){
                ans.str.push_back(sequence_[i]);
            }
            return ans;
        }

    private:
        long long len_, max_prod_, max_num_;
        std::pair<size_t, size_t> best_values_;
        std::vector<size_t> sequence_;
        SuffixTree tree_;

        int dfs_(int v, int curr_len) {
            int end_pos_cnt = 0;
            if (tree_.is_leaf(v)) {
                return 1;
            }
            for (int c = 0; c <= max_num_; c++) {
                int node = tree_.child(v, c);
                if (node == -1) {
                    continue;
                }
                end_pos_cnt += dfs_(node, curr_len + tree_.length(node));
            }
            if (v == 0) {
                return 0;
            }
            long long curr_prod = curr_len;
            curr_prod *= end_pos_cnt;
            if (curr_prod > max_prod_) {
                max_prod_ = curr_prod;
                best_values_.second = curr_len;
                best_values_.first = tree_.substr_end(v) - curr_len;
            }
            return end_pos_cnt;
        }
    };

    class AutomationSolver {
    public:
        AutomationSolver(data input):
            len_{input.len},
            max_prod_{0},
            max_num_{input.maxnum},
            best_values_{std::make_pair(0, 0)},
            sequence_{input.str},
            end_pos_cnt_{},
            first_pos_{},
            automation_{}
        {
        }

        Answer solve() {
            prepare_data_();
            precalc_end_pos_cnt_();
            max_prod_ = 0;
            for (int i = 0; i < automation_.size(); i++){
                long long curr_prod = end_pos_cnt_[i];
                int max_len = automation_.get_max_string_len(i);
                curr_prod *= max_len;
                if (curr_prod > max_prod_) {
                    best_values_ = std::make_pair(first_pos_[i] - max_len + 1, max_len);
                    max_prod_ = curr_prod;
                }
            }
            Answer ans;
            ans.refren = max_prod_;
            ans.len = best_values_.second;
            for (int i = best_values_.first; i < best_values_.first + best_values_.second; i++){
                ans.str.push_back(sequence_[i]);
            }
            return ans;
        }

    private:
    long long len_, max_prod_, max_num_;
    std::pair<size_t, size_t> best_values_;
    std::vector<size_t> sequence_;
    std::vector<size_t> end_pos_cnt_;
    std::vector<size_t> first_pos_;
    SuffixAutomation automation_;

    void prepare_data_() {
        end_pos_cnt_.push_back(0);
        first_pos_.push_back(0);
        for (int i = 0; i < len_; i++){
            automation_.add(sequence_[i]);
            end_pos_cnt_.push_back(1);
            first_pos_.push_back(i);
            if (automation_.size() == end_pos_cnt_.size() + 1) {
                end_pos_cnt_.push_back(0);
                first_pos_.push_back(i);
            }
        }
    }

    void precalc_end_pos_cnt_() {
        std::vector<std::pair<int, int>> ordered_by_maxlen_nodes;
        for (int i = 0; i < automation_.size(); i++){
            ordered_by_maxlen_nodes.push_back(std::make_pair(-automation_.get_max_string_len(i), i));
        }
        std::sort(ordered_by_maxlen_nodes.begin(), ordered_by_maxlen_nodes.end());
        for (auto pr: ordered_by_maxlen_nodes){
            int node = pr.second;
            int suff_parent = automation_.get_suff_link(node);
            if (suff_parent == -1) {
                continue;
            } else {
                end_pos_cnt_[suff_parent] += end_pos_cnt_[node];
            }
        }
    }
    };

    class ArraySolver {
    public:
        ArraySolver(data input):
            len_{input.len},
            max_prod_{0},
            max_num_{input.maxnum},
            best_values_{std::make_pair(0, 0)},
            sequence_{input.str},
            suff_array_{},
            lcp_{},
            array_{}
        {
        }

        Answer solve(){
            suff_array_ = array_.build_array(sequence_);
            lcp_ = array_.build_lcp();
            std::vector<std::pair<int, int>> sorted_lcp;
            for (int i = 0; i < sequence_.size()-1; i++) {
                sorted_lcp.push_back(std::make_pair(lcp_[i], i));
            }
            std::sort(sorted_lcp.begin(), sorted_lcp.end());
            std::set<int> indexes;
            max_prod_ = sequence_.size();
            best_values_ = std::make_pair(0, sequence_.size());
            indexes.insert(-1);
            indexes.insert(sequence_.size() - 1);
            for (auto p: sorted_lcp){
                int pos = p.second;
                int left_border = *(std::prev(indexes.lower_bound(pos)));
                int right_border = *(indexes.lower_bound(pos));
                long long curr_prod = right_border - left_border;
                curr_prod *= p.first;
                if (curr_prod > max_prod_) {
                    max_prod_ = curr_prod;
                    best_values_ = std::make_pair(suff_array_[left_border+1], p.first);
                }
                indexes.insert(pos);
            }
            Answer ans;
            ans.refren = max_prod_;
            ans.len = best_values_.second;
            for (int i = best_values_.first; i < best_values_.first + best_values_.second; i++){
                ans.str.push_back(sequence_[i]);
            }
            return ans;
        }

    private:
    long long len_, max_prod_, max_num_;
    std::pair<size_t, size_t> best_values_;
    std::vector<size_t> sequence_;
    std::vector<size_t> suff_array_;
    std::vector<size_t> lcp_;
    SuffixArray array_;
    };
}

NStringStructures::data read_input() {
    NStringStructures::data input;
    std::cin >> input.len >> input.maxnum;
    for (int i = 0; i < input.len; i++){
        int c;
        std::cin >> c;
        input.str.push_back(c);
    }
    return input;
}

void print_output(NStringStructures::Answer ans) {
    std::cout << ans.refren << std::endl;
    std::cout << ans.len << std::endl;
    for (size_t c: ans.str){
        std::cout << c << " ";
    }
}

int main(){
    NStringStructures::data input = read_input();
    NStringStructures::ArraySolver solver = NStringStructures::ArraySolver(input);
    NStringStructures::Answer ans = solver.solve();
    print_output(ans);
    return 0;
}
