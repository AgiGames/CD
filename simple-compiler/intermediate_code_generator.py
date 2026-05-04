import lexer
from typing import List

class IntermediateCodeGenerator:
    def __init__(self):
        self.code = []
        self.label_count = 0
        self.loop_stack = []

    def new_label(self):
        label = f"L{self.label_count}"
        self.label_count += 1
        return label

    def emit(self, instr):
        self.code.append(instr)

    def generate(self, tokens: List[lexer.Token]):
        i = 0
        n = len(tokens)

        while i < n:
            tok = tokens[i]

            if tok.type == 'int_variable_type':
                name = tokens[i+1].lexeme

                # check if assignment exists
                if tokens[i+2].type == 'assign_bin_operator':
                    value = tokens[i+3].lexeme
                    self.emit(("assign", name, value))
                    i += 5 # skip: int i = 5 ;
                else:
                    i += 3 # skip: int i ;

            elif tok.type == 'while':
                start_label = self.new_label()
                end_label = self.new_label()

                self.emit(("label", start_label))

                # condition: i <= 5
                left = tokens[i+2].lexeme
                op = tokens[i+3].lexeme
                right = tokens[i+4].lexeme

                cond = f"{left} {op} {right}"

                self.emit(("if_false", cond, end_label))

                # push loop info
                self.loop_stack.append((start_label, end_label))

                i += 6 # skip: while ( cond ) {

            elif tok.type == 'right_brace':
                if self.loop_stack:
                    start_label, end_label = self.loop_stack.pop()
                    self.emit(("goto", start_label))
                    self.emit(("label", end_label))
                i += 1

            elif tok.type == 'function' and tok.lexeme == 'printf':
                i += 2  # skip printf (

                arg_count = 0

                while tokens[i].type != 'right_parenthesis':
                    if tokens[i].type in ['string', 'identifier', 'int_constant']:
                        self.emit(("param", tokens[i].lexeme))
                        arg_count += 1
                    i += 1

                self.emit(("call", "printf", arg_count))
                i += 2  # skip ) ;

            elif tok.type == 'identifier' and tokens[i+1].type == 'increment_l_operator':
                name = tok.lexeme
                self.emit(("assign", name, f"{name} + 1"))
                i += 3

            elif tok.type == 'return':
                value = tokens[i+1].lexeme
                self.emit(("return", value))
                i += 3

            else:
                i += 1

        return self.code