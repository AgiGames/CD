#include "syntax_analyzer.hpp"

#include <stdexcept>

SyntaxAnalyzer::SyntaxAnalyzer() {
    /*
            program -> #include\<stdio.h> statement_list

            statement_list -> statement statement_list | eps
            statement -> int_variable_type identifier statement_tail |
                        while ( expression ) { statement_list } |
                        function ( args ) ; |
                        return expression ; |
                        expression ;
            statement_tail -> ; | = expression ; | ( ) { statement_list }

            expression -> factor term
            term -> bin_operator factor term | increment_l_operator term | eps
            factor -> int_constant | identifier

            bin_operator -> assign_bin_operator | less_or_equal_bin_operator

            args -> arg args_tail
            args_tail -> , arg args_tail | eps
            arg -> int_constant | string | identifier

            first(program) = [#include <stdio.h>]
            follow(program) = [$]

            first(statement_list) = [int_variable_type, while, function, return, idenitfier, int_constant, eps]
            follow(statement_list) = [$, }]

            first(statement) = [int_variable_type, while, function, return, identifier, int_constant]
            follow(statement) = [int_variable_type, while, function, return, identifier, int_constant, $, }]

            first(statement_tail) = [;, =, (]
            follow(statement_tail) = [int_variable_type, while, function, return, identifier, int_constant, $, }]

            first(expression) = [int_constant, identifier]
            follow(expression) = [), ;]

            first(factor) = [int_constant, identifier]
            follow(factor) = [bin_operator, increment_l_operator, ), ;]

            first(term) = [bin_operator, increment_l_operator, eps]
            follow(term) = [), ;]

            first(bin_operator) = [assign_bin_operator, less_or_equal_bin_operator]
            follow(bin_operator) = [int_constant, identifier]

            first(args) = [int_constant, string, identifier]
            follow(args) = [)]

            first(arg) = [int_constant, string, identifier]
            follow(arg) = [,, )]

            first(args_tail) = [,, eps]
            follow(args_tail) = [)]
    */
    table = {
        {"program", {
            {"#include <stdio.h>", {"#include <stdio.h>", "statement_list"}}
        }},

        {"statement_list", {
            {"int_variable_type", {"statement", "statement_list"}},
            {"while", {"statement", "statement_list"}},
            {"function", {"statement", "statement_list"}},
            {"return", {"statement", "statement_list"}},
            {"identifier", {"statement", "statement_list"}},
            {"int_constant", {"statement", "statement_list"}},
            {"}", {"eps"}},
            {"$", {"eps"}}
        }},

        {"statement", {
            {"int_variable_type", {"int_variable_type", "identifier", "statement_tail"}},
            {"while", {"while", "(", "expression", ")", "{", "statement_list", "}"}},
            {"function", {"function", "(", "args", ")", ";"}},
            {"return", {"return", "expression", ";"}},
            {"identifier", {"expression", ";"}},
            {"int_constant", {"expression", ";"}}
        }},

        {"statement_tail", {
            {";", {";"}},
            {"=", {"=", "expression", ";"}},
            {"(", {"(", ")", "{", "statement_list", "}"}}
        }},

        {"expression", {
            {"int_constant", {"factor", "term"}},
            {"identifier", {"factor", "term"}}
        }},

        {"term", {
            {"assign_bin_operator", {"bin_operator", "factor", "term"}},
            {"less_or_equal_bin_operator", {"bin_operator", "factor", "term"}},
            {"increment_l_operator", {"increment_l_operator", "term"}},
            {")", {"eps"}},
            {";", {"eps"}}
        }},

        {"factor", {
            {"int_constant", {"int_constant"}},
            {"identifier", {"identifier"}}
        }},

        {"bin_operator", {
            {"assign_bin_operator", {"assign_bin_operator"}},
            {"less_or_equal_bin_operator", {"less_or_equal_bin_operator"}}
        }},

        {"args", {
            {"int_constant", {"arg", "args_tail"}},
            {"string", {"arg", "args_tail"}},
            {"identifier", {"arg", "args_tail"}}
        }},

        {"args_tail", {
            {",", {",", "arg", "args_tail"}},
            {")", {"eps"}}
        }},

        {"arg", {
            {"int_constant", {"int_constant"}},
            {"string", {"string"}},
            {"identifier", {"identifier"}}
        }}
    };
}

bool SyntaxAnalyzer::parse(const std::vector<Token>& tokens) {
    std::vector<std::string> stack = {"$", "program"};
    int i = 0;

    while (!stack.empty()) {
        std::string top = stack.back();
        stack.pop_back();
        const Token& token = tokens[i];

        if (table.find(top) == table.end()) {
            if (top == token.lexeme || top == token.type) {
                i += 1;
            } else {
                throw std::runtime_error("Expected " + top + ", got " + token.lexeme + " at line " + std::to_string(token.line_no));
            }
        } else {
            std::vector<std::string> entry;
            bool found = false;

            auto non_terminal_it = table.find(top);
            if (non_terminal_it->second.find(token.lexeme) != non_terminal_it->second.end()) {
                entry = non_terminal_it->second[token.lexeme];
                found = true;
            } else if (non_terminal_it->second.find(token.type) != non_terminal_it->second.end()) {
                entry = non_terminal_it->second[token.type];
                found = true;
            }

            if (!found) {
                throw std::runtime_error("No rule for " + top + " with " + token.lexeme + " at line " + std::to_string(token.line_no));
            }

            if (!(entry.size() == 1 && entry[0] == "eps")) {
                for (auto it = entry.rbegin(); it != entry.rend(); ++it) {
                    stack.push_back(*it);
                }
            }
        }
    }

    return true;
}
