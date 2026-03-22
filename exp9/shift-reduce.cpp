#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

string input = "aabc$";
map<string, vector<string>> prod_rules = {
    {"S", {"aSb", "abS", "ab", ""}}
};

int main() {
    string stack = "$";
    size_t input_ptr = 0;
    cout << "Stack\t" << "Input\t" << "Action\t\n";

    while (input[input_ptr] != '$') {
        char cur_char = input[input_ptr];
        stack.push_back(cur_char);
        string action = "Shift ";
        action.push_back(cur_char);

        cout << stack.substr(0, stack.size()-1) << "\t" << input.substr(input_ptr) << "\t" << action << "\n";
        input_ptr += 1;

        for (const auto& [parent, children]: prod_rules) {
            for (const string child: children) {
                if (child.size() > stack.size()) continue;
                if (child == "" && stack == "$") {
                    stack.push_back('S');
                    cout << stack << "\t" << input.substr(input_ptr) << "\t" << action << "\n";
                    return 0;
                }

                size_t child_len = child.size();
                if (child != "" && child == stack.substr(stack.size() - child_len)) {
                    action = "Reduce " + parent + " -> " + child;
                    cout << stack << "\t" << input.substr(input_ptr) << "\t" << action << "\n";
                    stack.replace(stack.size() - child_len, child_len, parent);
                    break;
                }
            }
        }
    }

    if (stack == "$S") {
        cout << stack << "\t" << input.substr(input_ptr) << "\t" << "Accept" << "\n";
    }
    else {
        cout << stack << "\t" << input.substr(input_ptr) << "\t" << "reject" << "\n";
    }

    return 0;
}
