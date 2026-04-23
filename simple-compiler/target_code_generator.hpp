#ifndef TARGET_CODE_GENERATOR_HPP
#define TARGET_CODE_GENERATOR_HPP

#include <string>
#include <vector>

class TargetCodeGenerator {
public:
    TargetCodeGenerator();
    std::string new_reg();
    void emit(const std::string& instr);
    std::vector<std::string> generate(const std::vector<std::vector<std::string>>& ir_code);

private:
    std::vector<std::string> code;
    int reg_count;
};

#endif
