#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

string stack = "$";
string input = "";
vector<string> things;
unordered_map<char, size_t> arith_prec = {
    {'+', 0},
    {'-', 0},
    {'*', 1},
    {'/', 1}
};
char inter = 'A';

bool valid_char(char c) {
    if (c == '+' or c == '-' or c == '/' or c == '*' or c == '$') return true;
    if (c >= 'a' and c <= 'z') return true;
    return false;
}

bool is_id(char c) {
    if ((c >= 'a' and c <= 'z') ) return true;
    return false;
}

char precedence(char left, char right) {
    if ((is_id(left) and is_id(right)) or (left == '$' and right == '$')) return '=';
    if (is_id(left)) return '>';
    if (is_id(right)) return '<';
    if (left == '$') return '<';
    if (right == '$') return '>';
    if (arith_prec[left] < arith_prec[right]) return '<';
    if (arith_prec[left] > arith_prec[right]) return '>';
    return '>';
}

string create_postfix() {
    string postfix = "";
    for (char c: input) {
        if (!valid_char(c)) return "";
        char top = stack[stack.size() - 1];
        while (precedence(top, c) == '>') {
            postfix.push_back(top);
            stack.pop_back();
            top = stack[stack.size() - 1];
        }
        stack.push_back(c);
    }
    
    return postfix;
}

void create_things(string& postfix) {
    string stack = "";
    for (char c: postfix) {
        char top = stack.back();
        if (stack.size() > 0 and !is_id(top)) {
            string thing = {top, stack[stack.size() - 3], stack[stack.size() - 2], inter};
            things.push_back(thing);
            stack.pop_back();
            stack.pop_back();
            stack.pop_back();
            stack.push_back(inter++);
        }
        stack.push_back(c);
    }
    char top = stack.back();
    if (stack.size() > 0 and !is_id(top)) {
        string thing = {top, stack[stack.size() - 3], stack[stack.size() - 2], inter};
        things.push_back(thing);
        stack.pop_back();
        stack.pop_back();
        stack.pop_back();
        stack.push_back(inter++);
    }
}

void print_four_address(vector<string>& things) {
    cout << "\nFour Address Instructions\n";
    cout << "OPERATOR\tOPERAND 1\tOPERAND 2\tRESULT\n";
    for (string& thing: things) {
        char oper = thing[0];
        char operand1 = thing[1];
        char operand2 = thing[2];
        char result = thing[3];
        cout << oper << "\t\t" << operand1 << "\t\t" << operand2 << "\t\t" << result << "\n";
    }
}

void print_three_address(vector<string>& things) {
    cout << "\nThree Address Instructions\n";
    cout << "INDEX\tOPERATOR\tOPERAND 1\tOPERAND 2\n";
    for (string& thing: things) {
        char oper = thing[0];
        char operand1 = thing[1];
        char operand2 = thing[2];
        char result = thing[3];
        cout << (int) (result - 'A') << "\t" << oper << "\t\t";
        if (operand1 >= 'A' and operand1 <= 'Z') cout << "(" << (int) (operand1 - 'A') << ")" << "\t\t";
        else cout << operand1 << "\t\t";
        if (operand2 >= 'A' and operand2 <= 'Z') cout << "(" << (int) (operand2 - 'A') << ")" << "\n";
        else cout << operand1 << "\n";
    }
}

void print_indirect_three_address(vector<string>& things, size_t start) {
    cout << "\nIndirect Three Address Instructions\n";
    cout << "POINTER\tINDEX\n";
    for (string& thing: things) {
        char result = thing[3];
        cout << start << "\t" << (int) (result - 'A') << "\n";
        start += 1;
    }
}

int main() {
    cout << "Type in your input: ";
    cin >> input;
    input = input + '$';

    string postfix = create_postfix();
    cout << postfix << "\n";

    create_things(postfix);
    print_four_address(things);
    print_three_address(things);
    print_indirect_three_address(things, 34);

    return 0;
}
