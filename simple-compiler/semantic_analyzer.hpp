#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "lexer.hpp"

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    bool analyze(const std::vector<Token>& tokens);

private:
    std::unordered_map<std::string, std::pair<std::string, std::optional<int>>> symbol_table;
    std::unordered_map<std::string, std::vector<std::string>> functions;
};

#endif
