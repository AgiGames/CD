from typing import List
import lexer

class SyntaxAnalyzer():
    def __init__(self):
        '''
            program -> #include\<stdio.h> statement_list
            
            statement_list -> statement statement_list | eps
            statement -> int_variable_type identifier statement_tail |
                        while ( expression ) { statement_list } |
                        function ( args ) ; |
                        return expression ; |
                        expression ;
            statement_tail -> ; | = expression ; | ( ) { statement_list }
                        
            expression -> factor term
            term -> bin_operator factor term | increment_l_operator term | eps
            factor -> int_constant | identifier
            
            bin_operator -> assign_bin_operator | less_or_equal_bin_operator
            
            args -> arg args_tail
            args_tail -> , arg args_tail | eps
            arg -> int_constant | string | identifier
            
            first(program) = [#include <stdio.h>]
            follow(program) = [$]
            
            first(statement_list) = [int_variable_type, while, function, return, idenitfier, int_constant, eps]
            follow(statement_list) = [$, }]
            
            first(statement) = [int_variable_type, while, function, return, identifier, int_constant]
            follow(statement) = [int_variable_type, while, function, return, identifier, int_constant, $, }]
            
            first(statement_tail) = [;, =, (]
            follow(statement_tail) = [int_variable_type, while, function, return, identifier, int_constant, $, }]
            
            first(expression) = [int_constant, identifier]
            follow(expression) = [), ;]
            
            first(factor) = [int_constant, identifier]
            follow(factor) = [bin_operator, increment_l_operator, ), ;]
            
            first(term) = [bin_operator, increment_l_operator, eps]
            follow(term) = [), ;]
            
            first(bin_operator) = [assign_bin_operator, less_or_equal_bin_operator]
            follow(bin_operator) = [int_constant, identifier]
            
            first(args) = [int_constant, string, identifier]
            follow(args) = [)]
            
            first(arg) = [int_constant, string, identifier]
            follow(arg) = [,, )]
            
            first(args_tail) = [,, eps]
            follow(args_tail) = [)]
        '''
        self.table = {

            'program': {
                '#include <stdio.h>': ['#include <stdio.h>', 'statement_list']
            },

            'statement_list': {
                'int_variable_type': ['statement', 'statement_list'],
                'while': ['statement', 'statement_list'],
                'function': ['statement', 'statement_list'],
                'return': ['statement', 'statement_list'],
                'identifier': ['statement', 'statement_list'],
                'int_constant': ['statement', 'statement_list'],
                '}': ['eps'],
                '$': ['eps']
            },

            'statement': {
                'int_variable_type': ['int_variable_type', 'identifier', 'statement_tail'],
                'while': ['while', '(', 'expression', ')', '{', 'statement_list', '}'],
                'function': ['function', '(', 'args', ')', ';'],
                'return': ['return', 'expression', ';'],
                'identifier': ['expression', ';'],
                'int_constant': ['expression', ';']
            },

            'statement_tail': {
                ';': [';'],
                '=': ['=', 'expression', ';'],
                '(': ['(', ')', '{', 'statement_list', '}']
            },

            'expression': {
                'int_constant': ['factor', 'term'],
                'identifier': ['factor', 'term']
            },

            'term': {
                'assign_bin_operator': ['bin_operator', 'factor', 'term'],
                'less_or_equal_bin_operator': ['bin_operator', 'factor', 'term'],
                'increment_l_operator': ['increment_l_operator', 'term'],
                ')': ['eps'],
                ';': ['eps']
            },

            'factor': {
                'int_constant': ['int_constant'],
                'identifier': ['identifier']
            },

            'bin_operator': {
                'assign_bin_operator': ['assign_bin_operator'],
                'less_or_equal_bin_operator': ['less_or_equal_bin_operator']
            },

            'args': {
                'int_constant': ['arg', 'args_tail'],
                'string': ['arg', 'args_tail'],
                'identifier': ['arg', 'args_tail']
            },

            'args_tail': {
                ',': [',', 'arg', 'args_tail'],
                ')': ['eps']
            },

            'arg': {
                'int_constant': ['int_constant'],
                'string': ['string'],
                'identifier': ['identifier']
            }
        }
    
    def parse(self, tokens):
        stack = ['$', 'program']
        i = 0

        while stack:
            top = stack.pop()
            token = tokens[i]
            
            if top not in self.table:
                if top == token.lexeme or top == token.type:
                    i += 1
                else:
                    raise Exception(f"Expected {top}, got {token.lexeme} at line {token.line_no}")
            
            else:
                entry = None

                if token.lexeme in self.table[top]:
                    entry = self.table[top][token.lexeme]
                elif token.type in self.table[top]:
                    entry = self.table[top][token.type]

                if entry is None:
                    raise Exception(f"No rule for {top} with {token.lexeme} at line {token.line_no}")

                if entry != ['eps']:
                    stack.extend(reversed(entry))

        return True