#include "lexer.hpp"

#include <regex>
#include <fstream>
#include <stdexcept>

Token::Token(const std::string& type, const std::string& lexeme, int line_no)
    : type(type), lexeme(lexeme), line_no(line_no) {}

Lexer::Lexer(const std::string& file_path) : file_path(file_path) {
    regex_to_token_type = {
        {"#[^\\n]*", "preprocessor_line"},
        {"int", "int_variable_type"},
        {"while", "while"},
        {"return", "return"},
        {"printf", "function"},
        {"[a-zA-Z_][a-zA-Z0-9_]*", "identifier"},
        {"[0-9]+", "int_constant"},
        {"=", "assign_bin_operator"},
        {"<=", "less_or_equal_bin_operator"},
        {",", "comma"},
        {"\\+\\+", "increment_l_operator"},
        {"\\(", "left_parenthesis"},
        {"\\)", "right_parenthesis"},
        {"\\{", "left_brace"},
        {"\\}", "right_brace"},
        {"\"[^\"]*", "unterminated_string"},
        {"\".*\"", "string"},
        {";", "terminate"},
        {"[ \\t\\n]+", "whitespace"}
    };
}

std::string Lexer::get_matching_regex_pattern(const std::string& string) {
    for (const auto& pair : regex_to_token_type) {
        const std::string& regex_pattern = pair.first;
        if (std::regex_match(string, std::regex(regex_pattern))) {
            return regex_pattern;
        }
    }
    return "";
}

std::vector<Token> Lexer::tokenize() {
    std::string read_buffer = "";
    int line_count = 1;
    std::string candidate = "";

    std::ifstream f(file_path, std::ios::in);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    while (true) {
        char ch;
        f.get(ch);
        if (!f.good()) {
            break;
        }

        std::string char_str(1, ch);
        candidate = read_buffer + char_str;
        std::string matching_regex_pattern = get_matching_regex_pattern(candidate);

        if (!matching_regex_pattern.empty()) {
            read_buffer = candidate;
        } else {
            std::string prev_pattern = get_matching_regex_pattern(read_buffer);

            if (prev_pattern.empty()) {
                throw std::runtime_error("Invalid token at line " + std::to_string(line_count) + ": " + candidate);
            }

            std::string token_type;
            for (const auto& pair : regex_to_token_type) {
                if (pair.first == prev_pattern) {
                    token_type = pair.second;
                    break;
                }
            }

            if (token_type != "whitespace") {
                tokens.emplace_back(token_type, read_buffer, line_count);
            }

            read_buffer = char_str;
        }

        if (ch == '\n') {
            line_count += 1;
        }
    }

    if (!read_buffer.empty()) {
        std::string prev_pattern = get_matching_regex_pattern(read_buffer);

        if (prev_pattern.empty()) {
            throw std::runtime_error("Invalid token at line " + std::to_string(line_count) + ": " + candidate);
        }

        std::string token_type;
        for (const auto& pair : regex_to_token_type) {
            if (pair.first == prev_pattern) {
                token_type = pair.second;
                break;
            }
        }

        if (token_type != "whitespace") {
            tokens.emplace_back(token_type, read_buffer, line_count);
        }
    }

    tokens.emplace_back("$", "$", line_count);
    return tokens;
}
