#include "semantic_analyzer.hpp"

#include <stdexcept>

SemanticAnalyzer::SemanticAnalyzer() {
    symbol_table = {}; // holds (identifier, scope) -> (type, value)
    functions = {
        {"printf", {"string", "any*"}}
    };
}

bool SemanticAnalyzer::analyze(const std::vector<Token>& tokens) {
    int i = 0;
    int num_tokens = static_cast<int>(tokens.size());
    int scope_id = 0;
    std::vector<std::pair<std::string, int>> stack;
    std::unordered_map<std::string, int> scope; // holds how many times an identifier was defined

    while (i < num_tokens) {
        const Token& token = tokens[i];

        if (token.type == "left_brace") {
            scope_id += 1;

        } else if (token.type == "right_brace") {
            while (!stack.empty() && stack.back().second == scope_id) {
                scope[stack.back().first] -= 1;
                stack.pop_back();
            }
            scope_id -= 1;

        } else if (token.type == "int_variable_type") {
            bool is_variable_declaration = true;
            int j = i + 1;
            std::vector<Token> identifiers_like;
            while (tokens[j].type != "terminate") {
                if (tokens[j].type == "identifier") {
                    identifiers_like.push_back(tokens[j]);
                }
                if (tokens[j].type == "left_parenthesis" || tokens[j].type == "right_parenthesis") {
                    is_variable_declaration = false;
                    break;
                }
                j += 1;
            }

            if (is_variable_declaration) {
                for (const auto& identifier : identifiers_like) {
                    stack.push_back({identifier.lexeme, scope_id});
                    symbol_table[identifier.lexeme] = {"int", std::nullopt};
                    scope[identifier.lexeme] += 1;
                }
            }

        } else if (token.type == "assign_bin_operator") {
            const Token& prev_token = tokens[i - 1];
            const Token& next_token = tokens[i + 1];

            if (scope[prev_token.lexeme] == 0) {
                throw std::runtime_error(prev_token.lexeme + " not defined in this scope, at line " + std::to_string(prev_token.line_no) + ".");
            }
            if (prev_token.type != "identifier") {
                throw std::runtime_error("L value " + prev_token.lexeme + " is not a container, at line " + std::to_string(prev_token.line_no) + ".");
            }

            std::string prev_token_val_type = symbol_table[prev_token.lexeme].first;
            std::optional<int> prev_token_val = symbol_table[prev_token.lexeme].second;
            if (prev_token_val_type == "int" && next_token.type != "int_constant") {
                throw std::runtime_error("Type mismatch: " + prev_token.lexeme + " is not " + next_token.type + ", but rather " + prev_token_val_type + ", at line " + std::to_string(prev_token.line_no));
            }

            symbol_table[prev_token.lexeme] = {"int", std::stoi(next_token.lexeme)};

        } else if (token.type == "less_or_equal_bin_operator") {
            const Token& prev_token = tokens[i - 1];
            const Token& next_token = tokens[i + 1];

            if (scope[prev_token.lexeme] == 0) {
                throw std::runtime_error(prev_token.lexeme + " not defined in this scope, at line " + std::to_string(prev_token.line_no) + ".");
            }

            auto prev_token_val_type = symbol_table[prev_token.lexeme];
            if (next_token.type != "int_constant") {
                throw std::runtime_error("Type mismatch: " + prev_token.lexeme + " is not " + next_token.type + ", but rather " + prev_token_val_type.first + ", at line " + std::to_string(prev_token.line_no));
            }

        } else if (token.type == "function") {
            std::vector<std::string> function_signature = functions[token.lexeme];
            int signature_idx = 0;
            int j = i + 1;
            while (tokens[j].type != "right_parenthesis") {
                if (tokens[j].type == "comma" || tokens[j].type == "left_parenthesis") {
                    j += 1;
                    continue;
                }
                if (function_signature[signature_idx] == "any*" || function_signature[signature_idx] == "any" || function_signature[signature_idx] == tokens[j].type) {
                    if (function_signature[signature_idx] != "any*") {
                        signature_idx += 1;
                    }

                } else {
                    throw std::runtime_error(tokens[j].lexeme + " not compatible with function signature, expected " + function_signature[signature_idx] + ".");
                }

                j += 1;
            }
        }

        i += 1;
    }

    return true;
}
