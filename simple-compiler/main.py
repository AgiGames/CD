import lexer
import syntax_analyzer
import semantic_analyzer
import intermediate_code_generator
import target_code_generator

lxr = lexer.Lexer('input_file.c')
print('\n-----Lexemes-----')
tokens = lxr.tokenize()
print('lexeme\t\t\ttype')
for token in tokens:
    print(f'{token.lexeme}\t\t\t{token.type}')

sya = syntax_analyzer.SyntaxAnalyzer()
sya.parse(tokens)
print('\n-----Syntax Analysis-----')
print("Code is recognised to have correct syntax!")

sea = semantic_analyzer.SemanticAnalyzer()
sea.analyze(tokens)
print('\n-----Semantic Analysis-----')
print("Code is recognised to have corrent semantics!")

icg = intermediate_code_generator.IntermediateCodeGenerator()
intermediate_code = icg.generate(tokens)
print("\n-----Intermediate Code-----")
inter_var_count = 0
for line in intermediate_code:
    if line[0] == 'label':
        print(f'\n{line[1]}:')
    
    if line[0] == 'assign':
        print(f'{line[1]} = {line[2]}')
    
    if line[0] == 'if_false':
        print(f't{inter_var_count} = {line[1]}')
        print(f'{line[0]} t{inter_var_count} goto {line[2]}')
        inter_var_count += 1
    
    if line[0] == 'param' or line[0] == 'call' or line[0] == 'goto' or line[0] == 'return':
        for tok in line:
            print(tok, end=' ')
        print()

tcg = target_code_generator.TargetCodeGenerator()
print("\n-----Target Code-----")
target_code = tcg.generate(intermediate_code)
for line in target_code:
    for thing in line:
        print(thing, end='')
    print()

