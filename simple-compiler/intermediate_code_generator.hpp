#ifndef INTERMEDIATE_CODE_GENERATOR_HPP
#define INTERMEDIATE_CODE_GENERATOR_HPP

#include <string>
#include <vector>
#include "lexer.hpp"

class IntermediateCodeGenerator {
public:
    IntermediateCodeGenerator();
    std::string new_label();
    void emit(const std::vector<std::string>& instr);
    std::vector<std::vector<std::string>> generate(const std::vector<Token>& tokens);

private:
    std::vector<std::vector<std::string>> code;
    int label_count;
    std::vector<std::pair<std::string, std::string>> loop_stack;
};

#endif
