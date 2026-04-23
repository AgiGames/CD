#include "intermediate_code_generator.hpp"

IntermediateCodeGenerator::IntermediateCodeGenerator() {
    code = {};
    label_count = 0;
    loop_stack = {};
}

std::string IntermediateCodeGenerator::new_label() {
    std::string label = "L" + std::to_string(label_count);
    label_count += 1;
    return label;
}

void IntermediateCodeGenerator::emit(const std::vector<std::string>& instr) {
    code.push_back(instr);
}

std::vector<std::vector<std::string>> IntermediateCodeGenerator::generate(const std::vector<Token>& tokens) {
    int i = 0;
    int n = static_cast<int>(tokens.size());

    while (i < n) {
        const Token& tok = tokens[i];

        if (tok.type == "int_variable_type") {
            std::string name = tokens[i + 1].lexeme;

            // check if assignment exists
            if (tokens[i + 2].type == "assign_bin_operator") {
                std::string value = tokens[i + 3].lexeme;
                emit({"assign", name, value});
                i += 5; // skip: int i = 5 ;
            } else {
                i += 3; // skip: int i ;
            }

        } else if (tok.type == "while") {
            std::string start_label = new_label();
            std::string end_label = new_label();

            emit({"label", start_label});

            // condition: i <= 5
            std::string left = tokens[i + 2].lexeme;
            std::string op = tokens[i + 3].lexeme;
            std::string right = tokens[i + 4].lexeme;

            std::string cond = left + " " + op + " " + right;

            emit({"if_false", cond, end_label});

            // push loop info
            loop_stack.push_back({start_label, end_label});

            i += 6; // skip: while ( cond ) {

        } else if (tok.type == "right_brace") {
            if (!loop_stack.empty()) {
                std::string start_label = loop_stack.back().first;
                std::string end_label = loop_stack.back().second;
                loop_stack.pop_back();
                emit({"goto", start_label});
                emit({"label", end_label});
            }
            i += 1;

        } else if (tok.type == "function" && tok.lexeme == "printf") {
            i += 2;  // skip printf (

            int arg_count = 0;

            while (tokens[i].type != "right_parenthesis") {
                if (tokens[i].type == "string" || tokens[i].type == "identifier" || tokens[i].type == "int_constant") {
                    emit({"param", tokens[i].lexeme});
                    arg_count += 1;
                }
                i += 1;
            }

            emit({"call", "printf", std::to_string(arg_count)});
            i += 2;  // skip ) ;

        } else if (tok.type == "identifier" && tokens[i + 1].type == "increment_l_operator") {
            std::string name = tok.lexeme;
            emit({"assign", name, name + " + 1"});
            i += 3;

        } else if (tok.type == "return") {
            std::string value = tokens[i + 1].lexeme;
            emit({"return", value});
            i += 3;

        } else {
            i += 1;
        }
    }

    return code;
}
