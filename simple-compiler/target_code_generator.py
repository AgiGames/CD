class TargetCodeGenerator:
    def __init__(self):
        self.code = []
        self.reg_count = 0

    def new_reg(self):
        self.reg_count += 1
        return f"R{self.reg_count}"

    def emit(self, instr):
        self.code.append(instr)

    def generate(self, ir_code):
        for instr in ir_code:
            op = instr[0]

            if op == "assign":
                var = instr[1]
                value = instr[2]

                r = self.new_reg()
                self.emit(f"LOAD {r} {value}")
                self.emit(f"STORE {var} {r}")

            elif op == "label":
                self.emit(f"\n{instr[1]}:")

            elif op == "goto":
                self.emit(f"JMP {instr[1]}")

            elif op == "if_false":
                cond = instr[1]
                label = instr[2]

                left, operator, right = cond.split()

                r1 = self.new_reg()
                r2 = self.new_reg()

                self.emit(f"LOAD {r1} {left}")
                self.emit(f"LOAD {r2} {right}")
                self.emit(f"CMP {r1} {r2}")

                # invert condition for "if_false"
                if operator == "<=":
                    self.emit(f"JG {label}") # jump if greater -> false
                elif operator == "==":
                    self.emit(f"JNE {label}")
                else:
                    self.emit(f"; unsupported operator {operator}")

            elif op == "param":
                value = instr[1]
                r = self.new_reg()
                self.emit(f"LOAD {r} {value}")
                self.emit(f"PUSH {r}")

            elif op == "call":
                func = instr[1]
                n = instr[2]
                self.emit(f"CALL {func} {n}")

            elif op == "return":
                value = instr[1]
                r = self.new_reg()
                self.emit(f"LOAD {r} {value}")
                self.emit(f"RET {r}")

        return self.code