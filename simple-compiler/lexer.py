from typing import List
import re

class Token:
    def __init__(self, type, lexeme, line_no):
        self.type = type
        self.lexeme = lexeme
        self.line_no = line_no

class Lexer:
    def __init__(self, file_path):
        self.file_path = file_path
        self.regex_to_token_type = {
            r'#[^\n]*': 'preprocessor_line',
            r'int': 'int_variable_type',
            r'while': 'while',
            r'return': 'return',
            r'printf': 'function',
            r'[a-zA-Z_][a-zA-Z0-9_]*': 'identifier',
            r'[0-9]+': 'int_constant',
            r'=': 'assign_bin_operator',
            r'<=': 'less_or_equal_bin_operator',
            r',': 'comma',
            r'\+\+': 'increment_l_operator',
            r'\(': 'left_parenthesis',
            r'\)': 'right_parenthesis',
            r'\{': 'left_brace',
            r'\}': 'right_brace',
            r'"[^"]*': 'unterminated_string',
            r'".*"': 'string',
            r';': 'terminate',
            r'[ \t\n]+': 'whitespace',
        }
        self.regex_patterns = self.regex_to_token_type.keys()
        self.tokens = []

    def get_matching_regex_pattern(self, string: str):
        for regex_pattern in self.regex_patterns:
            if re.fullmatch(regex_pattern, string):
                return regex_pattern
        return None

    def tokenize(self):
        read_buffer = ""
        line_count = 1
        with open(self.file_path) as f:
            while True:
                char = f.read(1)
                if not char:
                    break
                
                candidate = read_buffer + char
                matching_regex_pattern = self.get_matching_regex_pattern(candidate)

                if matching_regex_pattern:
                    read_buffer = candidate
                else:
                    prev_pattern = self.get_matching_regex_pattern(read_buffer)

                    if prev_pattern is None:
                        raise Exception(f"Invalid token at line {line_count}: {candidate}")

                    token_type = self.regex_to_token_type[prev_pattern]
                    if token_type != 'whitespace':
                        self.tokens.append(Token(token_type, read_buffer, line_count))

                    read_buffer = char
                
                if char == '\n':
                    line_count += 1
                    
            if read_buffer:
                prev_pattern = self.get_matching_regex_pattern(read_buffer)

                if prev_pattern is None:
                    raise Exception(f"Invalid token at line {line_count}: {candidate}")

                token_type = self.regex_to_token_type[prev_pattern]
                if token_type != 'whitespace':
                    self.tokens.append(Token(token_type, read_buffer, line_count))
        
        self.tokens.append(Token('$', '$', line_count))
        return self.tokens