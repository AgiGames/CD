#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <deque>
#include <algorithm>

using namespace std;

#define EPS_CHAR '@'

bool is_non_term(char token) {
    return ('A' <= token && token <= 'Z');
}

bool is_term(char token) {
    return (token == EPS_CHAR) || !is_non_term(token);
}

map<string, vector<string>> grammar = {
    {"S", {"AA"}},
    {"A", {"aA", "b"}}
};
vector<string> non_terms = {"S", "A"};

size_t state_idx = 0;

typedef struct {
    string name;
    string parent_name;
    char trans_ele;
    map<string, vector<string>> s_grammar;
} State;
vector<State> states;

bool add_state(State s) {
    for (const State& si: states) {
        if (si.s_grammar == s.s_grammar) {
            return false;
        }
    }

    s.name = "I" + to_string(state_idx++);
    states.push_back(s);

    return true;
} 

map<string, vector<string>> closure(string non_term) {
    deque<string> q = {non_term};
    set<char> visited = {};
    
    map<string, vector<string>> closure_grammar = {};

    while (q.size() > 0) {
        string non_term = q.front();
        q.pop_front();

        vector<string> ith_prod_rules = grammar[non_term];
        for (string& jth_prod_rule: ith_prod_rules) {
            closure_grammar[non_term].push_back("." + jth_prod_rule);
            for (char c: jth_prod_rule) {
                if ((visited.find(c) == visited.end())  && is_non_term(c)) {
                    q.push_back(string(1, c));
                    visited.insert(c);
                }
            }
        }
    }

    return closure_grammar;
}

void initialize_parser() {
    non_terms.push_back("S");
    grammar["S'"].push_back("S"); // augmented it

    // make I0
    State I0 = {};
    I0.s_grammar = closure("S'");

    add_state(I0);
}

void merge_grammar(map<string, vector<string>>& destination, map<string, vector<string>>& source) {
    for (auto& [non_term, prod_rules]: source) {
        for (string prod_rule: prod_rules) {
            if (find(destination[non_term].begin(), destination[non_term].end(), prod_rule) == destination[non_term].end()) {
                destination[non_term].push_back(prod_rule);
            }
        }
    }
}

void compute_gotos() {
    bool changed = false;
    do {
        changed = false;
        vector<State> new_states;

        for (State& state: states) {
            string shiftables = "";
            for (auto& [non_term, prod_rules]: state.s_grammar) {
                for (string& prod_rule: prod_rules) {
                    size_t it = prod_rule.find('.');
                    if (it + 1 < prod_rule.size()) {
                        shiftables.push_back(prod_rule[it + 1]);
                    }
                }
            }

            for (size_t i = 0; i < shiftables.size(); ++i) {
                set<char> visited_closures = {};
                State new_state;
                new_state.parent_name = state.name;
                new_state.trans_ele = shiftables[i];
                bool need_this = false;

                char shiftable = shiftables[i];
                // cout << shiftable << "\n";
                for (auto& [non_term, prod_rules]: state.s_grammar) {
                    for (string& prod_rule: prod_rules) {
                        size_t it = prod_rule.find('.') + 1;
                        if (it < prod_rule.size() && prod_rule[it] == shiftable) {
                            need_this = true;
                            string shifted_prod_rule = prod_rule;
                            swap(shifted_prod_rule[it - 1], shifted_prod_rule[it]);
                            if (it + 1 < prod_rule.size() && is_non_term(prod_rule[it + 1])) {
                                if (visited_closures.find(prod_rule[it + 1]) == visited_closures.end()) {
                                    visited_closures.insert(prod_rule[it + 1]);
                                    map<string, vector<string>> closure_grammar = closure(string(1, prod_rule[it + 1]));
                                    
                                    merge_grammar(new_state.s_grammar, closure_grammar);
                                }
                            }
                            new_state.s_grammar[non_term].push_back(shifted_prod_rule);
                        }
                    }
                }

                if (need_this == true) {
                    new_states.push_back(new_state);      
                }
            }
        }

        for (const State ns: new_states) {
            changed = changed || add_state(ns);
        }

    } while (changed);
}

void print_states() {
    for (const State& state: states) {
        cout << "GOTO(" << state.parent_name << ", " << state.trans_ele << ") = " << state.name << ":\n";

        for (const auto& [non_term, prod_rules]: state.s_grammar) {
            cout << non_term << " -> ";
            for (size_t i = 0; i < prod_rules.size(); ++i) {
                cout << prod_rules[i];
                if (i != prod_rules.size() - 1) {
                    cout << " | ";
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }
}

int main() {
    initialize_parser();
    compute_gotos();
    print_states();

    return 0;
}
