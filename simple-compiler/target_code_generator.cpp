#include "target_code_generator.hpp"

#include <sstream>

TargetCodeGenerator::TargetCodeGenerator() {
    code = {};
    reg_count = 0;
}

std::string TargetCodeGenerator::new_reg() {
    reg_count += 1;
    return "R" + std::to_string(reg_count);
}

void TargetCodeGenerator::emit(const std::string& instr) {
    code.push_back(instr);
}

std::vector<std::string> TargetCodeGenerator::generate(const std::vector<std::vector<std::string>>& ir_code) {
    for (const auto& instr : ir_code) {
        std::string op = instr[0];

        if (op == "assign") {
            std::string var = instr[1];
            std::string value = instr[2];

            std::string r = new_reg();
            emit("LOAD " + r + " " + value);
            emit("STORE " + var + " " + r);

        } else if (op == "label") {
            emit("\n" + instr[1] + ":");

        } else if (op == "goto") {
            emit("JMP " + instr[1]);

        } else if (op == "if_false") {
            std::string cond = instr[1];
            std::string label = instr[2];

            std::string left;
            std::string operator_token;
            std::string right;
            std::istringstream iss(cond);
            iss >> left >> operator_token >> right;

            std::string r1 = new_reg();
            std::string r2 = new_reg();

            emit("LOAD " + r1 + " " + left);
            emit("LOAD " + r2 + " " + right);
            emit("CMP " + r1 + " " + r2);

            // invert condition for "if_false"
            if (operator_token == "<=") {
                emit("JG " + label); // jump if greater -> false
            } else if (operator_token == "==") {
                emit("JNE " + label);
            } else {
                emit("; unsupported operator " + operator_token);
            }

        } else if (op == "param") {
            std::string value = instr[1];
            std::string r = new_reg();
            emit("LOAD " + r + " " + value);
            emit("PUSH " + r);

        } else if (op == "call") {
            std::string func = instr[1];
            std::string n = instr[2];
            emit("CALL " + func + " " + n);

        } else if (op == "return") {
            std::string value = instr[1];
            std::string r = new_reg();
            emit("LOAD " + r + " " + value);
            emit("RET " + r);
        }
    }

    return code;
}
