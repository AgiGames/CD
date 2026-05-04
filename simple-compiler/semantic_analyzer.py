import lexer
from typing import List
from collections import defaultdict

class SemanticAnalyzer:
    def __init__(self):
        self.symbol_table = {} # holds (identifier, scope) -> (type, value)
        self.functions = {
            'printf': ['string', 'any*']
        }
    
    def analyze(self, tokens: List[lexer.Token]):
        i = 0
        num_tokens = len(tokens)
        scope_id = 0
        stack = []
        scope = defaultdict(int) # holds how many times an identifier was defined
                
        while i < num_tokens:
            token = tokens[i]
            
            if token.type == 'left_brace':
                scope_id += 1
                
            elif token.type == 'right_brace':
                while stack and stack[-1][1] == scope_id:
                    scope[stack[-1][0]] -= 1
                    stack.pop()
                scope_id -= 1
                
            elif token.type == 'int_variable_type':
                is_variable_declaration = True
                j = i + 1
                identifiers_like = []
                while tokens[j].type != 'terminate':
                    if tokens[j].type == 'identifier':
                        identifiers_like.append(tokens[j])
                    if tokens[j].type == 'left_parenthesis' or tokens[j].type == 'right_parenthesis':
                        is_variable_declaration = False
                        break
                    j += 1
                
                if is_variable_declaration:
                    for identifier in identifiers_like:
                        stack.append((identifier.lexeme, scope_id))
                        self.symbol_table[identifier.lexeme] = ('int', None)
                        scope[identifier.lexeme] += 1
                        
            elif token.type == 'assign_bin_operator':
                prev_token = tokens[i - 1]
                next_token = tokens[i + 1]
                
                if scope[prev_token.lexeme] == 0:
                    raise Exception(f'{prev_token.lexeme} not defined in this scope, at line {prev_token.line_no}.')
                if prev_token.type != 'identifier':
                    raise Exception(f'L value {prev_token.lexeme} is not a container, at line {prev_token.line_no}.')
                
                prev_token_val_type, prev_token_val = self.symbol_table[prev_token.lexeme]
                if prev_token_val_type == 'int' and next_token.type != 'int_constant':
                    raise Exception(f'Type mismatch: {prev_token.lexeme} is not {next_token.type}, but rather {prev_token_val_type}, at line {prev_token.line_no}')

                self.symbol_table[prev_token.lexeme] = ('int', int(next_token.lexeme))

            elif token.type == 'less_or_equal_bin_operator':
                prev_token = tokens[i - 1]
                next_token = tokens[i + 1]
                
                if scope[prev_token.lexeme] == 0:
                    raise Exception(f'{prev_token.lexeme} not defined in this scope, at line {prev_token.line_no}.')
                
                prev_token_val_type, prev_token_val = self.symbol_table[prev_token.lexeme]
                if prev_token_val_type == 'int' and next_token.type != 'int_constant':
                    raise Exception(f'Type mismatch: {prev_token.lexeme} is not {next_token.type}, but rather {prev_token_val_type}, at line {prev_token.line_no}')
            
            elif token.type == 'function':
                function_signature = self.functions[token.lexeme]
                signature_idx = 0
                j = i + 1
                while tokens[j].type != 'right_parenthesis':
                    if tokens[j].type == 'comma' or tokens[j].type == 'left_parenthesis':
                        j += 1
                        continue
                    if function_signature[signature_idx] == 'any*' or \
                        function_signature[signature_idx] == 'any' or function_signature[signature_idx] == tokens[j].type:
                        if function_signature[signature_idx] != 'any*':
                            signature_idx += 1
                    
                    else:
                        raise Exception(f'{tokens[j].lexeme} not compatible with function signature, expected {function_signature[signature_idx]}.')

                    j += 1
                    
            i += 1
        
        return True