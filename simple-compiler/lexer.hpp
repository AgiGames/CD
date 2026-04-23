#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <utility>

struct Token {
    std::string type;
    std::string lexeme;
    int line_no;

    Token(const std::string& type, const std::string& lexeme, int line_no);
};

class Lexer {
public:
    explicit Lexer(const std::string& file_path);
    std::string get_matching_regex_pattern(const std::string& string);
    std::vector<Token> tokenize();

private:
    std::string file_path;
    std::vector<std::pair<std::string, std::string>> regex_to_token_type;
    std::vector<Token> tokens;
};

#endif
