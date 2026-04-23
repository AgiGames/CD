#include <iostream>
#include <vector>
#include <string>

#include "lexer.hpp"
#include "syntax_analyzer.hpp"
#include "semantic_analyzer.hpp"
#include "intermediate_code_generator.hpp"
#include "target_code_generator.hpp"

int main() {
    Lexer lxr("input_file.c");
    std::vector<Token> tokens = lxr.tokenize();

    SyntaxAnalyzer sya;
    sya.parse(tokens);
    std::cout << "Code is recognised to have correct syntax!" << std::endl;

    SemanticAnalyzer sea;
    sea.analyze(tokens);
    std::cout << "Code is recognised to have corrent semantics!" << std::endl;

    IntermediateCodeGenerator icg;
    std::vector<std::vector<std::string>> intermediate_code = icg.generate(tokens);
    std::cout << "\n-----Intermediate Code-----" << std::endl;
    int inter_var_count = 0;
    for (const auto& line : intermediate_code) {
        if (line[0] == "label") {
            std::cout << "\n" << line[1] << ":" << std::endl;
        }

        if (line[0] == "assign") {
            std::cout << line[1] << " = " << line[2] << std::endl;
        }

        if (line[0] == "if_false") {
            std::cout << "t" << inter_var_count << " = " << line[1] << std::endl;
            std::cout << line[0] << " t" << inter_var_count << " goto " << line[2] << std::endl;
            inter_var_count += 1;
        }

        if (line[0] == "param" || line[0] == "call" || line[0] == "goto" || line[0] == "return") {
            for (const auto& tok : line) {
                std::cout << tok << " ";
            }
            std::cout << std::endl;
        }
    }

    TargetCodeGenerator tcg;
    std::cout << "\n-----Target Code-----" << std::endl;
    std::vector<std::string> target_code = tcg.generate(intermediate_code);
    for (const auto& line : target_code) {
        for (char thing : line) {
            std::cout << thing;
        }
        std::cout << std::endl;
    }

    return 0;
}
