#ifndef SYNTAX_ANALYZER_HPP
#define SYNTAX_ANALYZER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "lexer.hpp"

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer();
    bool parse(const std::vector<Token>& tokens);

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> table;
};

#endif
